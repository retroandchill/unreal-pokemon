// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Model/ManagedTestCase.h"

/**
 * @class FCSharpAutomationTest
 * @brief Represents an automation test class designed to interface with managed test cases.
 *
 * This class integrates Unreal Engine's automation test framework with managed test cases
 * provided via a handle. It provides functionality for defining, retrieving, and executing
 * individual tests while conforming to Unreal Engine's automation test workflow.
 */
class UNREALSHARPTEST_API FCSharpAutomationTest final : public FAutomationTestBase,
                                                        public TSharedFromThis<FCSharpAutomationTest>
{
  public:
    /**
     * @brief Constructs a new FCSharpAutomationTest instance with the specified test case handle.
     *
     * This constructor initializes an FCSharpAutomationTest object by associating it
     * with a provided managed test case handle. It sets up the test's fully qualified name
     * and performs necessary ownership and initialization tasks.
     *
     * @param TestCase The handle to the managed test case, containing details such as the
     * fully qualified name, assembly name, and relevant debugging information.
     */
    explicit FCSharpAutomationTest(FManagedTestCaseHandle TestCase)
        : FAutomationTestBase(TestCase.TestCase.FullyQualifiedName, false), ManagedTestCase(MoveTemp(TestCase))
    {
    }

    FString GetTestSourceFileName() const override;
    int32 GetTestSourceFileLine() const override;
    EAutomationTestFlags GetTestFlags() const override;
    FString GetBeautifiedTestName() const override;
    uint32 GetRequiredDeviceNum() const override;

  protected:
    void GetTests(TArray<FString> &OutBeautifiedNames, TArray<FString> &OutTestCommands) const override;
    bool RunTest(const FString &Parameters) override;

  private:
    FManagedTestCaseHandle ManagedTestCase;
};
