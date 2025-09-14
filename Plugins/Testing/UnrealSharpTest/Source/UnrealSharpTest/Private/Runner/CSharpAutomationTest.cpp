// Fill out your copyright notice in the Description page of Project Settings.

#include "Runner/CSharpAutomationTest.h"
#include "Interop/ManagedTestingCallbacks.h"
#include "UnrealSharpTestSettings.h"

FString FCSharpAutomationTest::GetTestSourceFileName() const
{
    return ManagedTestCase.TestCase.CodeFilePath;
}

int32 FCSharpAutomationTest::GetTestSourceFileLine() const
{
    return ManagedTestCase.TestCase.LineNumber;
}

EAutomationTestFlags FCSharpAutomationTest::GetTestFlags() const
{
    return EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter;
}

FString FCSharpAutomationTest::GetBeautifiedTestName() const
{
    if (const auto Settings = GetDefault<UUnrealSharpTestSettings>(); !Settings->GetSharedPrefix().IsEmpty())
    {
        return Settings->GetSharedPrefix() + TEXT(".") + ManagedTestCase.TestCase.FullyQualifiedName;
    }
    return ManagedTestCase.TestCase.FullyQualifiedName;
}

uint32 FCSharpAutomationTest::GetRequiredDeviceNum() const
{
    return 1;
}

void FCSharpAutomationTest::GetTests(TArray<FString> &OutBeautifiedNames, TArray<FString> &OutTestCommands) const
{
    FManagedTestingCallbacks::Get().GetTests(ManagedTestCase.ManagedTestCase.GetHandle(), OutBeautifiedNames,
                                             OutTestCommands);
}

bool FCSharpAutomationTest::RunTest(const FString &Parameters)
{
    return FManagedTestingCallbacks::Get().RunTest(*this, ManagedTestCase.ManagedTestCase.GetHandle(),
                                                   FName(Parameters));
}
