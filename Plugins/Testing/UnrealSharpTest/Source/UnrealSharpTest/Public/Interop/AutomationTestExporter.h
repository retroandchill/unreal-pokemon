// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CSBindsManager.h"
#include "CSManagedGCHandle.h"
#include "UObject/Object.h"

#include "AutomationTestExporter.generated.h"

class FCSharpAutomationTest;

/**
 * @class UAutomationTestExporter
 * @brief A utility class for handling automation test-related functionalities in Unreal Engine.
 *
 * This class provides static methods for adding test cases, queuing latent commands, and logging automation test
 * events.
 */
UCLASS()
class UNREALSHARPTEST_API UAutomationTestExporter : public UObject
{
    GENERATED_BODY()

  public:
    /**
     * @brief Adds a new test case to the automation test framework.
     *
     * Appends a beautified test case name and its corresponding command to the provided output arrays.
     *
     * @param ParameterName The unique parameter name identifier associated with the test case.
     * @param BeautifiedName A human-readable name for the test case.
     * @param OutBeautifiedNames An array to store beautified names for test cases.
     * @param OutTestCommands An array to store commands associated with the test cases.
     */
    UNREALSHARP_FUNCTION()
    static void AddTestCase(FName ParameterName, const TCHAR *BeautifiedName, TArray<FString> &OutBeautifiedNames,
                            TArray<FString> &OutTestCommands);

    /**
     * @brief Queues a latent automation test command for asynchronous execution.
     *
     * This method schedules a new latent command to be executed in the automation test framework.
     * The latent command is based on the provided test instance and task pointer.
     *
     * @param Test A pointer to the automation test instance that owns this latent command. Must not be nullptr.
     * @param TaskPtr A handle to the managed task providing the functionality to be executed within the latent command.
     */
    UNREALSHARP_FUNCTION()
    static void EnqueueLatentCommand(FCSharpAutomationTest *Test, FGCHandleIntPtr TaskPtr);

    /**
     * @brief Logs an automation event with a specified message and event type.
     *
     * This method records the automation event to the current test if one is active,
     * or logs it directly through Unreal Engine's logging system if no active test exists.
     *
     * @param Message The message associated with the automation event.
     * @param EventType The type of the automation event being logged.
     */
    UNREALSHARP_FUNCTION()
    static void LogEvent(const TCHAR *Message, EAutomationEventType EventType);

    /**
     * @brief Registers an expected log message for the current automation test.
     *
     * This method adds an expected log message to the active automation test, indicating the conditions under which
     * the message should be matched. If no test is active, an error is logged.
     *
     * @param ExpectedPattern The pattern to match the expected message against.
     * @param ExpectedVerbosity The verbosity level of the expected message.
     * @param MatchType The criteria for matching the expected message (e.g., exact match, contains, etc.).
     * @param Occurrences The number of times the message is expected to occur.
     * @param IsRegex Specifies whether the pattern should be treated as a regular expression.
     */
    UNREALSHARP_FUNCTION()
    static void AddExpectedMessage(const TCHAR *ExpectedPattern, ELogVerbosity::Type ExpectedVerbosity,
                                   EAutomationExpectedMessageFlags::MatchType MatchType, int32 Occurrences,
                                   bool IsRegex);
};
