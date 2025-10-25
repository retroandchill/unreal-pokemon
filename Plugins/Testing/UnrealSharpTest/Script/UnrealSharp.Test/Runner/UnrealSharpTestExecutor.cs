using System.Reflection;
using NUnit.Framework;
using NUnit.Framework.Interfaces;
using NUnit.Framework.Internal;
using UnrealSharp.Core;
using UnrealSharp.CoreUObject;
using UnrealSharp.Test.Interop;
using UnrealSharp.Test.Model;

namespace UnrealSharp.Test.Runner;

/// <summary>
/// Provides methods to execute and log test cases for UnrealSharp automation testing framework.
/// This static class is intended to manage the execution of test cases, handling test results,
/// and managing test class instances during the testing process.
/// </summary>
public static class UnrealSharpTestExecutor
{
    private static readonly Dictionary<Type, object?> TestClassInstances = new();
    private static readonly Dictionary<ParameterInfo, object?[]> EvaluatedParameters = new();

    internal static void ClearTestClassInstances()
    {
        TestClassInstances.Clear();
        EvaluatedParameters.Clear();
    }

    /// <summary>
    /// Executes a test method within the current process, scheduling it for execution if it is asynchronous.
    /// </summary>
    /// <param name="automationTestReference">
    /// A reference to the automation test, encapsulating native test execution details.
    /// </param>
    /// <param name="testMethod">
    /// The test method to be executed, which includes setup, teardown, and test method metadata.
    /// </param>
    /// <param name="testCase">
    /// The test case identifier, represented by a name to distinguish individual test runs.
    /// </param>
    /// <returns>
    /// A boolean value indicating whether the test execution was successfully completed synchronously
    /// or scheduled for asynchronous execution.
    /// </returns>
    public static bool RunTestInProcess(
        AutomationTestRef automationTestReference,
        UnrealTestMethod testMethod,
        FName testCase
    )
    {
        var testTask = RunTestInProcessInternal(testMethod, testCase);
        if (testTask.IsCompletedSuccessfully)
        {
            return testTask.Result;
        }

        automationTestReference.EnqueueNativeTask(testTask.AsTask());
        return true;
    }

    private static async ValueTask<bool> RunTestInProcessInternal(UnrealTestMethod testMethod, FName testCase)
    {
        using var nunitContext = new TestExecutionContext.IsolatedContext();
        var testResult = TestExecutionContext.CurrentContext.CurrentResult;

        try
        {
            var testClass = testMethod.Method.DeclaringType;
            ArgumentNullException.ThrowIfNull(testClass);

            if (!TestClassInstances.TryGetValue(testClass, out var testInstance))
            {
                testInstance = Activator.CreateInstance(testClass);
                TestClassInstances[testClass] = testInstance;
            }

            try
            {
                await RunTestMethod(testInstance, testMethod.SetupMethod);

                var arguments = testMethod.TestCases.GetValueOrDefault(testCase);
                var result = await RunTestMethod(testInstance, testMethod.Method, arguments);

                if (arguments?.HasExpectedResult == true)
                {
                    Assert.That(result, Is.EqualTo(arguments.ExpectedResult));
                }
            }
            catch (Exception e)
            {
                testResult.RecordException(e);
            }
            finally
            {
                try
                {
                    await RunTestMethod(testInstance, testMethod.TearDownMethod);
                }
                catch (Exception e)
                {
                    testResult.RecordTearDownException(e);
                }
            }
        }
        catch (Exception e)
        {
            testResult.RecordException(e);
        }

        LogTestResult(testResult);
        return testResult.ResultState.Status != TestStatus.Failed;
    }

    private static object?[] GetArguments(this TestCaseData testCaseData)
    {
        return testCaseData
            .Arguments.Select(x =>
                x is IDataPlaceholder placeholder ? placeholder.GetArguments()[placeholder.Index] : x
            )
            .ToArray();
    }

    private static object?[] GetArguments(this IDataPlaceholder placeholder)
    {
        if (EvaluatedParameters.TryGetValue(placeholder.ParameterInfo, out var arguments))
        {
            return arguments;
        }

        var newArguments = placeholder.GetData().Cast<object>().ToArray();
        EvaluatedParameters[placeholder.ParameterInfo] = newArguments;
        return newArguments;
    }

    /// <summary>
    /// Logs the test result, including any messages or assertion details, for reporting and debugging purposes.
    /// </summary>
    /// <param name="result">
    /// The test result object containing the outcome, messages, and assertion details of the executed test case.
    /// </param>
    public static void LogTestResult(TestResult result)
    {
        if (!string.IsNullOrWhiteSpace(result.Message) && result.AssertionResults.All(r => r.Message != result.Message))
        {
            LogTestMessage(result.Message);
        }

        foreach (var assertion in result.AssertionResults)
        {
            var eventType = assertion.Status switch
            {
                AssertionStatus.Inconclusive or AssertionStatus.Passed => EAutomationEventType.Info,
                AssertionStatus.Warning => EAutomationEventType.Warning,
                AssertionStatus.Failed or AssertionStatus.Error => EAutomationEventType.Error,
                _ => throw new InvalidOperationException("Unknown assertion status"),
            };

            var message =
                assertion.Status == AssertionStatus.Error
                    ? $"{assertion.Message}\n{assertion.StackTrace}"
                    : assertion.Message;
            LogTestMessage(message, eventType);
        }
    }

    private static void LogTestMessage(string message, EAutomationEventType eventType = EAutomationEventType.Info)
    {
        unsafe
        {
            fixed (char* messagePtr = message)
            {
                AutomationTestExporter.CallLogEvent(messagePtr, eventType);
            }
        }
    }

    private static async ValueTask<object?> RunTestMethod(
        object? testFixture,
        MethodInfo? methodInfo,
        TestCaseData? testCaseData = null
    )
    {
        if (methodInfo is null)
            return null;
        try
        {
            var arguments = testCaseData?.GetArguments() ?? [];
            var result = methodInfo.Invoke(testFixture, arguments);
            switch (result)
            {
                case null:
                    return null;
                case Task task:
                {
                    await task;
                    if (!task.GetType().IsGenericType || task.GetType().GetGenericTypeDefinition() != typeof(Task<>))
                        return null;

                    var awaitMethod = typeof(UnrealSharpTestExecutor)
                        .GetMethod(nameof(AwaitTask), BindingFlags.NonPublic | BindingFlags.Static)!
                        .MakeGenericMethod(task.GetType().GenericTypeArguments[0]);

                    var awaitTask = (ValueTask<object?>)awaitMethod.Invoke(null, [task])!;
                    return await awaitTask;
                }
                case ValueTask valueTask:
                    await valueTask;
                    return null;
            }

            var resultType = result.GetType();
            if (!resultType.IsGenericType)
                return result;

            var genericTypeDefinition = resultType.GetGenericTypeDefinition();
            if (genericTypeDefinition != typeof(ValueTask<>))
                return result;

            var valueAwaitMethod = typeof(UnrealSharpTestExecutor)
                .GetMethod(nameof(AwaitValueTask), BindingFlags.NonPublic | BindingFlags.Static)!
                .MakeGenericMethod(resultType.GenericTypeArguments[0]);

            var awaitValueTask = (ValueTask<object?>)valueAwaitMethod.Invoke(null, [result])!;
            return await awaitValueTask;
        }
        catch (TargetInvocationException e)
        {
            if (e.InnerException is null)
            {
                throw new InvalidOperationException("Test method threw an exception but no inner exception was set", e);
            }

            throw e.InnerException;
        }
    }

    private static async ValueTask<object?> AwaitTask<T>(Task<T> task)
    {
        return await task;
    }

    private static async ValueTask<object?> AwaitValueTask<T>(ValueTask<T> valueTask)
    {
        return await valueTask;
    }
}
