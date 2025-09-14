// Fill out your copyright notice in the Description page of Project Settings.

#include "Interop/ManagedTestingExporter.h"
#include "CSManager.h"
#include "UnrealSharpTest.h"

void UManagedTestingExporter::SetManagedActions(const FManagedTestingActions &InActions)
{
    FManagedTestingCallbacks::Get().SetActions(InActions);
}

FGCHandleIntPtr UManagedTestingExporter::FindUserAssembly(const FName AssemblyName)
{
    const auto Assembly = UCSManager::Get().FindAssembly(AssemblyName);
    if (Assembly == nullptr)
        return FGCHandleIntPtr();
    return Assembly->GetManagedAssemblyHandle()->GetHandle();
}

void UManagedTestingExporter::AddTestCase(TArray<FManagedTestCaseHandle> &TestCases, FManagedTestCase &TestCase,
                                          FGCHandleIntPtr ManagedTest)
{
    TestCases.Emplace(MoveTemp(TestCase), ManagedTest);
}
