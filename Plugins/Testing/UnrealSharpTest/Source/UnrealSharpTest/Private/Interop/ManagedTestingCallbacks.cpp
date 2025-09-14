// Fill out your copyright notice in the Description page of Project Settings.

#include "Interop/ManagedTestingCallbacks.h"
#include "Runner/CSharpAutomationTest.h"

FManagedTestingCallbacks &FManagedTestingCallbacks::Get()
{
    static FManagedTestingCallbacks Instance;
    return Instance;
}

void FManagedTestingCallbacks::SetActions(const FManagedTestingActions &InActions)
{
    Actions = InActions;
}

TArray<FManagedTestCaseHandle> FManagedTestingCallbacks::CollectTestCases(
    const TConstArrayView<FName> AssemblyPaths) const
{
    TArray<FManagedTestCaseHandle> Result;
    Actions.CollectTestCases(AssemblyPaths.GetData(), AssemblyPaths.Num(), &Result);
    return Result;
}

void FManagedTestingCallbacks::GetTests(const FGCHandleIntPtr ManagedTest, TArray<FString> &OutBeautifiedNames,
                                        TArray<FString> &OutTestCommands) const
{
    Actions.GetTests(ManagedTest, &OutBeautifiedNames, &OutTestCommands);
}

bool FManagedTestingCallbacks::RunTest(FCSharpAutomationTest &Test, const FGCHandleIntPtr ManagedTest,
                                       const FName TestCase) const
{
    return Actions.RunTest(&Test, ManagedTest, TestCase);
}

bool FManagedTestingCallbacks::CheckTaskComplete(const FSharedGCHandle &Task) const
{
    return Actions.CheckTaskComplete(Task.GetHandle());
}

void FManagedTestingCallbacks::ClearTestClassInstances() const
{
    Actions.ClearTestClassInstances();
}
