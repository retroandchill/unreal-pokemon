using NUnit.Framework.Internal;
using UnrealSharp.Attributes;
using UnrealSharp.FunctionalTesting;
using UnrealSharp.Test.Runner;

namespace UnrealSharp.Test.Functional;

/// <summary>
/// Represents an asynchronous functional test base class for creating functional tests
/// where the test code includes async logic. It is derived from the AFunctionalTest class
/// and is intended to handle UnrealSharp-specific functional testing workflows.
/// </summary>
/// <remarks>
/// This class provides functionality for executing asynchronous tests using the
/// NUnit testing framework, capturing any exceptions, and logging results through
/// the UnrealSharpTestExecutor.
/// </remarks>
[UClass]
public class AAsyncFunctionalTest : AFunctionalTest
{
    /// <inheritdoc />
    protected sealed override void StartTest()
    {
        _ = RunTestInternal();
    }

    private async Task RunTestInternal()
    {
        using var nunitContext = new TestExecutionContext.IsolatedContext();
        var testResult = TestExecutionContext.CurrentContext.CurrentResult;

        try
        {
            await RunTest();
        }
        catch (Exception e)
        {
            testResult.RecordException(e);
        }

        UnrealSharpTestExecutor.LogTestResult(testResult);
    }

    /// <summary>
    /// Executes the core logic for the test asynchronously. Override this method to define specific
    /// test logic for asynchronous functional tests.
    /// </summary>
    /// <returns>A <see cref="ValueTask"/> representing the asynchronous operation of the test logic.</returns>
    protected virtual ValueTask RunTest()
    {
        return ValueTask.CompletedTask;
    }
}
