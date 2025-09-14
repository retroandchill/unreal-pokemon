using System.Runtime.InteropServices;
using JetBrains.Annotations;
using UnrealSharp.Core;
using UnrealSharp.Test.Discovery;
using UnrealSharp.Test.Mappers;
using UnrealSharp.Test.Model;
using UnrealSharp.Test.Runner;
using UnrealSharp.UnrealSharpTest;

namespace UnrealSharp.Test.Interop;

/// <summary>
/// Represents a structure that defines a set of unmanaged function pointers for interacting
/// with managed testing operations. This structure acts as a bridge between managed and
/// unmanaged code, providing the necessary functionality for invoking test-related tasks.
/// </summary>
/// <remarks>
/// The <c>ManagedTestingActions</c> structure encapsulates delegates pointing to unmanaged functions
/// that handle test case collection, test execution, completion checks, and test instance management.
/// These functions are typically used in a cross-language testing environment where interop between
/// managed and unmanaged systems is required.
/// </remarks>
[StructLayout(LayoutKind.Sequential)]
public unsafe struct ManagedTestingActions
{
#pragma warning disable CS1572 // XML comment has a param tag, but there is no parameter by that name
    /// <summary>
    /// Represents an unmanaged function pointer used for collecting test cases from a specified set
    /// of assemblies. This delegate facilitates communication between managed and unmanaged environments
    /// to retrieve available test cases.
    /// </summary>
    /// <remarks>
    /// This function pointer is typically invoked with a pointer to an array of assembly names, the
    /// length of that array, and an unmanaged output array to receive the collected test cases. It serves
    /// as part of the interop infrastructure for testing frameworks that operate across managed and unmanaged
    /// contexts.
    /// </remarks>
    /// <param name="assemblyNamesPtr">A pointer to the memory location containing an array of assembly names.</param>
    /// <param name="assemblyNamesLength">The number of assembly names provided in the array.</param>
    /// <param name="outputArrayPtr">A pointer to an unmanaged array where the collected test cases will be stored.</param>
    [UsedImplicitly]
    public required delegate* unmanaged<
        IntPtr,
        int,
        UnmanagedArray*,
        void> CollectTestCases { get; init; }

    /// <summary>
    /// Represents an unmanaged function pointer used for retrieving tests and their associated metadata
    /// from a specified managed test case. This delegate provides interop functionality for accessing
    /// test information in a cross-language testing environment.
    /// </summary>
    /// <remarks>
    /// This function pointer is typically invoked with a pointer to a managed test case, along with
    /// unmanaged arrays to store the beautified test names and their respective parameters. It serves
    /// as part of the infrastructure enabling communication between managed and unmanaged testing components.
    /// </remarks>
    /// <param name="managedTestCasePtr">A pointer to the managed test case from which tests are retrieved.</param>
    /// <param name="beautifiedNames">A pointer to an unmanaged array where beautified test names will be stored.</param>
    /// <param name="testParameters">A pointer to an unmanaged array where the parameters for each test will be stored.</param>
    [UsedImplicitly]
    public required delegate* unmanaged<
        IntPtr,
        UnmanagedArray*,
        UnmanagedArray*,
        void> GetTests { get; init; }

    /// <summary>
    /// Represents an unmanaged function pointer used for executing a specific test case.
    /// This delegate allows managed and unmanaged systems to invoke the execution of
    /// a test by referencing its native and managed representations and its associated name.
    /// </summary>
    /// <remarks>
    /// The function pointer is typically called with a reference to the native test object,
    /// the managed test case object, and the name of the test to be executed. It enables
    /// seamless execution of tests in a cross-language environment.
    /// </remarks>
    /// <param name="nativeTest">A reference to the unmanaged representation of the test.</param>
    /// <param name="managedTestCasePtr">A pointer to the managed object representing the test case.</param>
    /// <param name="testName">The name of the test case to be executed.</param>
    /// <returns>
    /// A <c>NativeBool</c> indicating the success or failure of the test execution.
    /// </returns>
    [UsedImplicitly]
    public required delegate* unmanaged<IntPtr, IntPtr, FName, NativeBool> RunTest { get; init; }

    /// <summary>
    /// Represents an unmanaged function pointer used to check if a given task has been completed.
    /// This delegate facilitates communication between managed and unmanaged environments to monitor
    /// task progress or completion status.
    /// </summary>
    /// <remarks>
    /// This function pointer is typically invoked with a single argument representing the state or context
    /// of the task being checked. It acts as part of the interop infrastructure for verification and synchronization
    /// in cross-language testing scenarios.
    /// </remarks>
    /// <param name="taskStatePtr">A pointer to the memory location representing the state or context of the task.</param>
    /// <returns>A boolean value indicating whether the task has been completed.</returns>
    [UsedImplicitly]
    public required delegate* unmanaged<IntPtr, NativeBool> CheckTaskComplete { get; init; }

    /// <summary>
    /// Represents an unmanaged function pointer used to clear or reset test class instances
    /// within the testing framework. This delegate facilitates cleanup operations by releasing
    /// resources or resetting states associated with generated test classes.
    /// </summary>
    /// <remarks>
    /// This function pointer is typically invoked in scenarios where test class instances
    /// need to be disposed or reset to ensure a clean testing environment. It is often used
    /// in testing workflows to prevent residual state or memory issues between test runs.
    /// </remarks>
    [UsedImplicitly]
    public required delegate* unmanaged<void> ClearTestClassInstances { get; init; }
#pragma warning restore CS1572 // XML comment has a param tag, but there is no parameter by that name

    /// <summary>
    /// Creates and initializes a new instance of the ManagedTestingActions structure using predefined unmanaged function pointers for testing operations.
    /// </summary>
    /// <returns>
    /// A new instance of the ManagedTestingActions structure with all delegate pointers initialized to the respective unmanaged functions.
    /// </returns>
    public static ManagedTestingActions Create()
    {
        return new ManagedTestingActions
        {
            CollectTestCases = &ManagedTestingCallbacks.CollectTestCases,
            GetTests = &ManagedTestingCallbacks.GetTests,
            RunTest = &ManagedTestingCallbacks.RunTest,
            CheckTaskComplete = &ManagedTestingCallbacks.CheckTaskComplete,
            ClearTestClassInstances = &ManagedTestingCallbacks.ClearTestClassInstances,
        };
    }
}

/// <summary>
/// Provides a set of unmanaged static functions utilized for performing key testing operations
/// within the UnrealSharp testing framework. This class allows for seamless communication
/// between managed and unmanaged environments.
/// </summary>
/// <remarks>
/// The <c>ManagedTestingCallbacks</c> class includes static unmanaged functions to handle various
/// operations such as collecting test cases, retrieving test details, executing tests, checking
/// task completion, and clearing test class instances. These functions are usually called from
/// an unmanaged codebase that requires interop with the managed test infrastructure.
/// </remarks>
public static unsafe class ManagedTestingCallbacks
{
    /// <summary>
    /// Collects test cases from specified assemblies and stores them in an unmanaged structure.
    /// </summary>
    /// <param name="assemblyNamesPtr">
    /// Pointer to an array containing the names of assemblies to search for test cases.
    /// </param>
    /// <param name="assemblyNamesLength">
    /// The number of entries in the assembly names array.
    /// </param>
    /// <param name="outputArrayPtr">
    /// Pointer to an unmanaged array where the collected test cases will be stored.
    /// </param>
    [UnmanagedCallersOnly]
    public static void CollectTestCases(
        IntPtr assemblyNamesPtr,
        int assemblyNamesLength,
        UnmanagedArray* outputArrayPtr
    )
    {
        var testCases = new ReadOnlySpan<FName>((FName*)assemblyNamesPtr, assemblyNamesLength);

        var nativeStruct = stackalloc byte[FManagedTestCaseMarshaller.GetNativeDataSize()];
        foreach (var testCase in UnrealSharpTestDiscoveryClient.DiscoverTests(testCases))
        {
            var unrealStruct = testCase.ToManagedTestCase();
            unrealStruct.ToNative((IntPtr)nativeStruct);

            var testCaseHandle = GCHandle.Alloc(testCase);
            var testCaseHandlePtr = GCHandle.ToIntPtr(testCaseHandle);

            ManagedTestingExporter.CallAddTestCase(
                ref *outputArrayPtr,
                (IntPtr)nativeStruct,
                testCaseHandlePtr
            );
        }
    }

    /// <summary>
    /// Populates arrays with test case metadata such as names and parameters for a specified managed test case.
    /// </summary>
    /// <param name="managedTestCasePtr">
    /// A pointer to the managed test case object from which the test data is retrieved.
    /// </param>
    /// <param name="beautifiedNames">
    /// A pointer to an unmanaged array that will be populated with the formatted names of the test cases.
    /// </param>
    /// <param name="testParameters">
    /// A pointer to an unmanaged array that will be populated with the parameters associated with each test case.
    /// </param>
    [UnmanagedCallersOnly]
    public static void GetTests(
        IntPtr managedTestCasePtr,
        UnmanagedArray* beautifiedNames,
        UnmanagedArray* testParameters
    )
    {
        var managedTestCase = GCHandleUtilities.GetObjectFromHandlePtr<UnrealTestMethod>(
            managedTestCasePtr
        );
        if (managedTestCase is null)
        {
            LogUnrealSharpTest.LogError("Failed to get tests");
            return;
        }

        if (managedTestCase.TestCases.Count == 0)
        {
            fixed (char* testNamePtr = managedTestCase.FullyQualifiedName)
            {
                AutomationTestExporter.CallAddTestCase(
                    FName.None,
                    testNamePtr,
                    ref *beautifiedNames,
                    ref *testParameters
                );
            }
        }

        var testIndex = 1;
        var digitsCount = (int)Math.Log10(managedTestCase.TestCases.Count) + 1;
        foreach (var (testCommand, testCase) in managedTestCase.TestCases)
        {
            var index = testIndex.ToString().PadLeft(digitsCount, '0');
            fixed (char* displayNamePtr = $"#{index}: {testCase.GetDisplayName()}")
            {
                AutomationTestExporter.CallAddTestCase(
                    testCommand,
                    displayNamePtr,
                    ref *beautifiedNames,
                    ref *testParameters
                );
            }
            testIndex++;
        }
    }

    /// <summary>
    /// Executes a test operation by invoking the provided unmanaged and managed test cases.
    /// </summary>
    /// <param name="nativeTest">
    /// A pointer to the native test object to be executed.
    /// </param>
    /// <param name="managedTestCasePtr">
    /// A pointer to the managed test case instance wrapped in a GCHandle.
    /// </param>
    /// <param name="testName">
    /// The name of the test to be executed, provided as an FName structure.
    /// </param>
    /// <returns>
    /// A <see cref="NativeBool"/> value indicating whether the test execution completed successfully.
    /// </returns>
    [UnmanagedCallersOnly]
    public static NativeBool RunTest(IntPtr nativeTest, IntPtr managedTestCasePtr, FName testName)
    {
        var testCase = GCHandleUtilities.GetObjectFromHandlePtr<UnrealTestMethod>(
            managedTestCasePtr
        );
        if (testCase is null)
        {
            return NativeBool.False;
        }

        return UnrealSharpTestExecutor
            .RunTestInProcess(new AutomationTestRef(nativeTest), testCase, testName)
            .ToNativeBool();
    }

    /// <summary>
    /// Checks whether a previously initiated task is complete and handles its result, logging any exceptions if the task has faulted.
    /// </summary>
    /// <param name="taskHandlePtr">
    /// A pointer to the handle of the managed task being checked for completion.
    /// </param>
    /// <returns>
    /// A NativeBool indicating whether the task is completed. Returns true (NativeBool.True) if the task is complete,
    /// false (NativeBool.False) if the task is still running.
    /// </returns>
    [UnmanagedCallersOnly]
    public static NativeBool CheckTaskComplete(IntPtr taskHandlePtr)
    {
        var task = GCHandleUtilities.GetObjectFromHandlePtr<Task>(taskHandlePtr);
        ArgumentNullException.ThrowIfNull(task);
        if (!task.IsCompleted)
            return NativeBool.False;

        if (task.IsFaulted)
        {
            LogUnrealSharpTest.LogError(task.Exception!.ToString());
        }
        task.Dispose();
        return NativeBool.True;
    }

    /// <summary>
    /// Releases and clears all test class instances that were allocated during test execution, ensuring no lingering state persists across test runs.
    /// </summary>
    [UnmanagedCallersOnly]
    public static void ClearTestClassInstances()
    {
        UnrealSharpTestExecutor.ClearTestClassInstances();
    }
}
