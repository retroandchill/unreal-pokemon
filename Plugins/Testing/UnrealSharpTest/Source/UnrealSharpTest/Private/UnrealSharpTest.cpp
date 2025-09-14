// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnrealSharpTest.h"
#include "CSManager.h"
#include "Interop/ManagedTestingCallbacks.h"
#include "Runner/CSharpAutomationTest.h"
#include "UnrealSharpProcHelper/CSProcHelper.h"

#define LOCTEXT_NAMESPACE "FUnrealSharpTestModule"

DEFINE_LOG_CATEGORY(LogUnrealSharpTestNative)

FUnrealSharpTestModule *FUnrealSharpTestModule::Instance = nullptr;

void FUnrealSharpTestModule::StartupModule()
{
    Instance = this;
    RegisterTestsDelegateHandle = FCoreDelegates::OnAllModuleLoadingPhasesComplete.AddLambda([this] {
        RegisterTests();

        auto &Manager = UCSManager::Get();
        Manager.OnManagedAssemblyLoadedEvent().AddRaw(this, &FUnrealSharpTestModule::RegisterTests);
        Manager.OnManagedAssemblyUnloadedEvent().AddRaw(this, &FUnrealSharpTestModule::UnregisterTests);
    });

    ClearTestCacheDelegateHandle = FAutomationTestFramework::Get().OnAfterAllTestsEvent.AddLambda(
        [] { FManagedTestingCallbacks::Get().ClearTestClassInstances(); });
}

void FUnrealSharpTestModule::ShutdownModule()
{
    Instance = nullptr;
    FAutomationTestFramework::Get().OnAfterAllTestsEvent.Remove(ClearTestCacheDelegateHandle);
    FCoreDelegates::OnAllModuleLoadingPhasesComplete.Remove(RegisterTestsDelegateHandle);
}

void FUnrealSharpTestModule::RegisterTests()
{
    TArray<FString> Paths;
    FCSProcHelper::GetAssemblyPathsByLoadOrder(Paths);

    TArray<FName> Assemblies;
    Assemblies.Reserve(Paths.Num());
    for (const auto &Path : Paths)
    {
        int32 ExtensionMarker;
        if (!Path.FindLastChar(TEXT('.'), ExtensionMarker))
        {
            ExtensionMarker = Path.Len() - 1;
        }
        const int32 Separator = Path.FindLastCharByPredicate([](const TCHAR Ch) { return Ch == '/' || Ch == '\\'; });

        FStringView FileNameView(*Path + Separator + 1, ExtensionMarker - Separator - 1);
        Assemblies.Emplace(FileNameView);
    }

    RegisterTests(Assemblies);
}

void FUnrealSharpTestModule::RegisterTests(const FName &AssemblyName)
{
    TStaticArray<FName, 1> Assemblies = {AssemblyName};
    RegisterTests(Assemblies);
}

void FUnrealSharpTestModule::RegisterTests(const TConstArrayView<FName> Assemblies)
{
    const auto &Callbacks = FManagedTestingCallbacks::Get();
    if (!Callbacks.IsValid())
        return;

    auto &TestFramework = FAutomationTestFramework::Get();
    for (auto TestCases = Callbacks.CollectTestCases(Assemblies); auto &TestCase : TestCases)
    {
        auto &AssemblyList = Tests.FindOrAdd(TestCase.TestCase.AssemblyName);
        auto &Test = AssemblyList.Emplace_GetRef(MakeShared<FCSharpAutomationTest>(MoveTemp(TestCase)));
        TestFramework.RegisterAutomationTest(Test->GetTestFullName(), &Test.Get());
    }
}

void FUnrealSharpTestModule::UnregisterTests(const FName &AssemblyName)
{
    const auto AssemblyList = Tests.Find(AssemblyName);
    if (AssemblyList == nullptr)
        return;

    auto &TestFramework = FAutomationTestFramework::Get();
    for (const auto &Test : *AssemblyList)
    {
        TestFramework.UnregisterAutomationTest(Test->GetTestFullName());
    }
    Tests.Remove(AssemblyName);
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FUnrealSharpTestModule, UnrealSharpTest)