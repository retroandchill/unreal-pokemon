// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CSManagedGCHandle.h"
#include "Model/ManagedTestCase.h"

class FCSharpAutomationTest;

/**
 * FManagedTestingActions defines a structure that provides a set of function pointers
 * for managing and interacting with test cases in a C# managed environment from C++.
 * These actions include collecting test cases, retrieving test metadata, running tests,
 * checking task completion, and clearing test class instances.
 *
 * The structure bridges the communication with managed assemblies using callable
 * function pointers, which are used by other components, such as callbacks and exporters,
 * to interface between the managed and unmanaged execution contexts.
 */
struct FManagedTestingActions
{
    using FCollectTestCases = void(__stdcall *)(const FName *, int32, TArray<FManagedTestCaseHandle> *);
    using FGetTests = void(__stdcall *)(FGCHandleIntPtr, TArray<FString> *, TArray<FString> *);
    using FRunTest = bool(__stdcall *)(FCSharpAutomationTest *, FGCHandleIntPtr, FName);
    using FCheckTaskComplete = bool(__stdcall *)(FGCHandleIntPtr);
    using FClearTestClassInstances = void(__stdcall *)();

    /**
     * CollectTestCases is a function pointer that references a callable method
     * responsible for gathering test cases from managed assemblies. The function
     * takes an array of assembly names, the count of these assemblies, and a reference
     * to a collection where the found test case handles will be stored.
     *
     * This is utilized to iterate through assemblies in a managed environment
     * and collect metadata about the available test cases, represented as
     * FManagedTestCaseHandle instances.
     */
    FCollectTestCases CollectTestCases = nullptr;

    /**
     * FGetTests is a function pointer type used to reference a callable method
     * in a managed environment that retrieves a list of test cases and their
     * associated metadata. The function takes a handle to the managed test instance,
     * along with references to two arrays: one for storing the beautified test names
     * and another for storing the corresponding test commands.
     *
     * This function is primarily used to query the managed assemblies for information
     * about available test cases, where the retrieved details can be utilized for
     * organization, display, or further processing within the unmanaged environment.
     *
     * When invoked, the method locates all available tests linked to the given
     * managed test instance and populates the provided arrays with the results.
     */
    FGetTests GetTests = nullptr;

    /**
     * FRunTest is a function pointer that references a callable method in a managed environment
     * responsible for executing a specific test case. The function takes a reference to an
     * FCSharpAutomationTest instance, a handle to the associated managed test object
     * (FGCHandleIntPtr), and the name of the test case to be executed (FName).
     *
     * This function is used to initiate the execution of a test case defined in managed
     * code from an unmanaged environment. It facilitates interoperation by bridging
     * the execution contexts, ensuring that the test can run seamlessly within the
     * managed environment while being triggered from native code.
     *
     * When called, this function triggers the execution of the specified test case
     * and returns a boolean result indicating whether the test was successfully initiated.
     *
     * Defaults to nullptr until assigned a valid implementation.
     */
    FRunTest RunTest = nullptr;

    /**
     * CheckTaskComplete is a function pointer of type FCheckTaskComplete used to reference a callable
     * method in a managed environment. This function is designed to check the completion status of a
     * specific asynchronous task, identified by its handle (FGCHandleIntPtr).
     *
     * This pointer facilitates communication between unmanaged and managed execution contexts,
     * enabling the native code to query and determine if the specified task has completed execution
     * within the managed runtime. When called, the function evaluates the state of the task and returns
     * a boolean result indicating whether the task is complete.
     *
     * Defaults to nullptr until assigned a valid implementation.
     */
    FCheckTaskComplete CheckTaskComplete = nullptr;

    /**
     * FClearTestClassInstances defines a function pointer type for invoking a callable
     * method in a managed environment that clears instances of test classes. This is used
     * to free resources associated with test class objects in the managed runtime, ensuring
     * proper cleanup and resource management.
     *
     * This function pointer is typically utilized after test execution or during teardown
     * to reset the state of the managed environment by disposing of any lingering instances
     * of test classes. It bridges the unmanaged and managed code by allowing unmanaged
     * environments to trigger cleanup operations within the managed runtime.
     *
     * Defaults to nullptr until assigned a valid implementation.
     */
    FClearTestClassInstances ClearTestClassInstances = nullptr;
};

/**
 * FManagedTestingCallbacks is a singleton class that encapsulates testing callbacks
 * for managing test cases, their execution, and management of related tasks.
 * It provides methods to collect, execute, and handle validation for test cases
 * while interfacing with managed assemblies.
 */
class UNREALSHARPTEST_API FManagedTestingCallbacks
{
    FManagedTestingCallbacks() = default;
    ~FManagedTestingCallbacks() = default;

  public:
    /**
     * Retrieves the singleton instance of the FManagedTestingCallbacks class, ensuring there is
     * only one instance of this class throughout the application lifecycle.
     *
     * This function is primarily used for managing the callbacks and interactions
     * with test cases and related actions in the managed environment.
     *
     * It employs a static local variable to guarantee thread-safe initialization
     * and consistent access to the same instance.
     *
     * @return A reference to the singleton instance of FManagedTestingCallbacks.
     */
    static FManagedTestingCallbacks &Get();

    /**
     * Checks whether the contained FManagedTestingActions has a valid CollectTestCases function pointer.
     *
     * This function verifies if the CollectTestCases function pointer in the Actions member is not null,
     * signaling that the testing actions have been properly initialized and are operational.
     *
     * @return true if the CollectTestCases function pointer is not null; false otherwise.
     */
    bool IsValid() const
    {
        return Actions.CollectTestCases != nullptr;
    }

    /**
     * Sets the current managed testing actions to the specified instance of FManagedTestingActions.
     *
     * This method assigns a new set of function pointers encapsulated within FManagedTestingActions
     * to enable or update the interaction and functionality related to managed test cases.
     * This is utilized to replace or initialize the callable actions that interact with the
     * managed test environment.
     *
     * @param InActions A reference to an FManagedTestingActions instance containing the
     *                  function pointers for managing and interacting with test cases.
     */
    void SetActions(const FManagedTestingActions &InActions);

    /**
     * Collects test cases from the specified assemblies and returns them as an array
     * of FManagedTestCaseHandle instances. Each test case handle represents a managed
     * test case along with its corresponding shared GC handle for lifecycle management.
     *
     * This function interacts with the managed environment to retrieve the list of
     * test cases, leveraging the CollectTestCases function pointer defined in the
     * FManagedTestingActions structure.
     *
     * @param AssemblyPaths A constant array view of FName objects representing
     *                      the paths or identifiers of assemblies to collect test cases from.
     * @return An array of FManagedTestCaseHandle instances representing the collected test cases.
     */
    TArray<FManagedTestCaseHandle> CollectTestCases(TConstArrayView<FName> AssemblyPaths) const;

    /**
     * Retrieves a collection of test names and corresponding test commands for a given managed test instance.
     *
     * This function interacts with the underlying managed environment associated with the given managed
     * test handle to extract test metadata. The extracted data includes readable test names that can be
     * displayed to the user (e.g., in a UI or console) and the corresponding commands used to execute those tests.
     *
     * @param ManagedTest A handle to the managed test instance from which test metadata will be retrieved.
     *                     This handle links to a managed object in the C# environment.
     * @param OutBeautifiedNames An output array to hold the human-readable names of the tests retrieved
     *                            from the managed test instance.
     * @param OutTestCommands An output array to hold the commands or identifiers associated with each test,
     *                         which can be used for executing the corresponding test cases.
     */
    void GetTests(FGCHandleIntPtr ManagedTest, TArray<FString> &OutBeautifiedNames,
                  TArray<FString> &OutTestCommands) const;

    /**
     * Executes a specified test case associated with a managed test instance.
     *
     * This method interacts with the managed environment through the provided function
     * pointers in FManagedTestingActions to execute a test case. It uses the specified
     * FCSharpAutomationTest instance for test execution, the ManagedTest handle linking
     * to the managed test environment, and the name of the specific test case to run.
     *
     * @param Test A reference to an FCSharpAutomationTest instance that handles
     *             the automation of the test execution in Unreal Engine.
     * @param ManagedTest An FGCHandleIntPtr handle referencing the managed test instance
     *                     in the C# environment.
     * @param TestCase The FName representing the specific test case to be executed.
     * @return true if the test case was executed successfully; false otherwise.
     */
    bool RunTest(FCSharpAutomationTest &Test, FGCHandleIntPtr ManagedTest, FName TestCase) const;

    /**
     * CheckTaskComplete determines whether a specific managed task, identified by
     * its shared garbage-collected handle, has completed execution.
     *
     * This method utilizes the associated actions provided in the structure to
     * check the task's completion state by interacting with the managed environment.
     * It facilitates communication between the unmanaged and managed contexts.
     *
     * @param Task The shared garbage-collected handle representing the managed task
     *             to be checked for completion.
     * @return A boolean value indicating whether the task has been completed
     *         (true if completed; false otherwise).
     */
    bool CheckTaskComplete(const FSharedGCHandle &Task) const;

    /**
     * ClearTestClassInstances is a member function of the FManagedTestingCallbacks class,
     * responsible for invoking the action to clear instantiated test class objects.
     *
     * This function facilitates the removal of managed test class instances from memory
     * in the underlying managed runtime, ensuring proper resource cleanup between
     * test executions or when transitioning execution phases.
     *
     * The method is designed to call the associated function pointer provided by the
     * Actions structure, effectively bridging the unmanaged and managed execution contexts.
     */
    void ClearTestClassInstances() const;

  private:
    FManagedTestingActions Actions;
};
