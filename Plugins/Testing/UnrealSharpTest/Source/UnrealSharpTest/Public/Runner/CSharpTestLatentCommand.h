// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CSManagedGCHandle.h"
#include "Model/ManagedTestCase.h"

class FCSharpAutomationTest;

/**
 * @class FCSharpTestLatentCommand
 * @brief Represents a latent automation command for C# testing in Unreal Engine.
 *
 * This class is used to execute a task provided by a managed C# test environment
 * in the context of Unreal Engine's automation testing framework.
 * The task is tracked as a C# GC handle and verifies completion to
 * proceed with the automation pipeline.
 *
 * @details
 * This class inherits from the IAutomationLatentCommand interface and is designed
 * to execute asynchronous or deferred commands as part of a test sequence in Unreal Engine.
 * The class is final, meaning it cannot be further inherited.
 *
 * The Update function is called continuously during the automation testing phase
 * until the task is marked complete.
 *
 * @note
 * This class interacts with both C++ and managed C# components
 * through GC handles or weak pointers for resource management and reliability.
 *
 * @see
 * - IAutomationLatentCommand
 * - FManagedTestingCallbacks
 * - FCSharpAutomationTest
 * - FSharedGCHandle
 */
class UNREALSHARPTEST_API FCSharpTestLatentCommand final : public IAutomationLatentCommand
{
  public:
    /**
     * @brief Constructs a new FCSharpTestLatentCommand instance.
     *
     * This constructor initializes the latent command with a weak pointer
     * to the owning automation test and a managed task handle.
     * The task handle represents a managed C# task that will be executed
     * within Unreal Engine's automation testing framework.
     *
     * @param Owner A weak pointer to the FCSharpAutomationTest instance that owns this latent command.
     *              This parameter is used to ensure the owning instance is valid while preventing circular
     * dependencies.
     * @param TestTask A handle to the managed C# task associated with this command that must be tracked
     *                 and executed as part of the automation test sequence.
     */
    FCSharpTestLatentCommand(TWeakPtr<FCSharpAutomationTest> Owner, FGCHandleIntPtr TestTask);

    bool Update() override;

  private:
    TWeakPtr<FCSharpAutomationTest> Owner;
    FSharedGCHandle TestTask;
};
