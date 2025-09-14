// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CSManagedGCHandle.h"

#include "ManagedTestCase.generated.h"

/**
 * @struct FManagedTestCase
 * @brief A structure representing a managed test case within the testing framework.
 *
 * This structure encapsulates metadata about a single test case, including its assembly name,
 * fully qualified name, source file path, and line number. It is primarily used to map and
 * manage test cases originating from a managed environment.
 */
USTRUCT(BlueprintType)
struct FManagedTestCase
{
    GENERATED_BODY()

    /**
     * @var AssemblyName
     * @brief The name of the assembly containing the test case.
     *
     * This variable holds the name of the assembly to which the test case belongs.
     * It serves as a key to group and manage test cases originating from the same assembly
     * within the testing framework.
     */
    UPROPERTY(BlueprintReadOnly)
    FName AssemblyName;

    /**
     * @var FullyQualifiedName
     * @brief The fully qualified name of the managed test case.
     *
     * This variable stores the uniquely qualified name of a test case as defined in the managed environment.
     * It allows the identification and association of test cases within the testing framework and external systems.
     */
    UPROPERTY(BlueprintReadOnly)
    FString FullyQualifiedName;

    /**
     * @var CodeFilePath
     * @brief The source file path of the managed test case.
     *
     * This variable contains the file path to the source code where the managed test case
     * is defined. It enables traceability and assists in debugging by providing a direct
     * reference to the location of the test case definition in the codebase.
     */
    UPROPERTY(BlueprintReadOnly)
    FString CodeFilePath;

    /**
     * @var LineNumber
     * @brief The line number in the source file where the test case is defined.
     *
     * This variable specifies the exact line in the source code associated with the managed test case definition.
     * It helps in locating the precise position of the test case in the file during debugging or test analysis.
     */
    UPROPERTY(BlueprintReadOnly)
    int32 LineNumber;
};

struct FManagedTestCaseHandle
{
    const FManagedTestCase TestCase;
    const FSharedGCHandle ManagedTestCase;

    FManagedTestCaseHandle(FManagedTestCase InTestCase, const FGCHandleIntPtr InManagedTestCase)
        : TestCase(MoveTemp(InTestCase)), ManagedTestCase(InManagedTestCase)
    {
    }
};