// Fill out your copyright notice in the Description page of Project Settings.

#include "Interop/AutomationTestExporter.h"
#include "Runner/CSharpAutomationTest.h"
#include "Runner/CSharpTestLatentCommand.h"
#include "UnrealSharpTest.h"

class FFunctionalTestBase;

void UAutomationTestExporter::AddTestCase(const FName ParameterName, const TCHAR *BeautifiedName,
                                          TArray<FString> &OutBeautifiedNames, TArray<FString> &OutTestCommands)
{
    OutBeautifiedNames.Emplace(BeautifiedName);
    OutTestCommands.Emplace(ParameterName.IsNone() ? "" : ParameterName.ToString());
}

void UAutomationTestExporter::EnqueueLatentCommand(FCSharpAutomationTest *Test, const FGCHandleIntPtr TaskPtr)
{
    check(Test != nullptr);

    ADD_LATENT_AUTOMATION_COMMAND(FCSharpTestLatentCommand(Test->AsShared(), TaskPtr));
}

void UAutomationTestExporter::LogEvent(const TCHAR *Message, const EAutomationEventType EventType)
{
    if (const auto CurrentTest = FAutomationTestFramework::Get().GetCurrentTest(); CurrentTest != nullptr)
    {
        CurrentTest->AddEvent(FAutomationEvent(EventType, Message));
    }
    else
    {
        UE_LOG(LogUnrealSharpTestNative, Warning,
               TEXT("Tried to log a message for a C# test outside of the testing framework."));

        switch (EventType)
        {
        case EAutomationEventType::Info:
            UE_LOG(LogUnrealSharpTestNative, Display, TEXT("%s"), Message);
            break;
        case EAutomationEventType::Warning:
            UE_LOG(LogUnrealSharpTestNative, Warning, TEXT("%s"), Message);
            break;
        case EAutomationEventType::Error:
            UE_LOG(LogUnrealSharpTestNative, Error, TEXT("%s"), Message);
            break;
        }
    }
}

void UAutomationTestExporter::AddExpectedMessage(const TCHAR *ExpectedPattern, ELogVerbosity::Type ExpectedVerbosity,
                                                 EAutomationExpectedMessageFlags::MatchType MatchType,
                                                 int32 Occurrences, bool IsRegex)
{
    if (const auto CurrentTest = FAutomationTestFramework::Get().GetCurrentTest(); CurrentTest != nullptr)
    {
        CurrentTest->AddExpectedMessage(ExpectedPattern, ExpectedVerbosity, MatchType, Occurrences, IsRegex);
    }
    else
    {
        UE_LOG(LogUnrealSharpTestNative, Error,
               TEXT("Tried to record an expected message outside of the testing framework."));
    }
}
