// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ManagedTestingCallbacks.h"
#include "Model/ManagedTestCase.h"
#include "UnrealSharpBinds/Public/CSBindsManager.h"
#include "UObject/Object.h"

#include "ManagedTestingExporter.generated.h"

/**
 * @class UManagedTestingExporter
 * @brief A utility class designed to manage interaction with managed testing functionalities.
 *
 * This class provides static methods to configure testing actions, handle user assemblies,
 * and manage individual test cases for managed testing frameworks.
 */
UCLASS()
class UNREALSHARPTEST_API UManagedTestingExporter : public UObject
{
    GENERATED_BODY()

  public:
    /**
     * @brief Sets the managed testing actions to be used.
     *
     * This method allows the user to configure and set the actions that should be
     * used for managing testing functionalities within the managed testing framework.
     *
     * @param InActions The set of managed testing actions to be applied.
     */
    UNREALSHARP_FUNCTION()
    static void SetManagedActions(const FManagedTestingActions &InActions);

    /**
     * @brief Finds a user assembly by its name and retrieves its managed handle.
     *
     * This method searches for a user-defined assembly within the managed environment
     * by its name and returns a handle to the corresponding managed assembly if found.
     *
     * @param AssemblyName The name of the user assembly to locate.
     * @return FGCHandleIntPtr A handle to the managed assembly, or an empty handle if the assembly is not found.
     */
    UNREALSHARP_FUNCTION()
    static FGCHandleIntPtr FindUserAssembly(FName AssemblyName);

    /**
     * @brief Adds a new test case to the collection of managed test cases.
     *
     * This method inserts a new test case into the provided array of managed test case handles.
     * Each test case is paired with its corresponding managed test handle during the insertion.
     *
     * @param TestCases The array where the new test case will be added.
     * @param TestCase The test case to be added to the collection.
     * @param ManagedTest The handle to the managed test case associated with the test case.
     */
    UNREALSHARP_FUNCTION()
    static void AddTestCase(TArray<FManagedTestCaseHandle> &TestCases, FManagedTestCase &TestCase,
                            FGCHandleIntPtr ManagedTest);
};
