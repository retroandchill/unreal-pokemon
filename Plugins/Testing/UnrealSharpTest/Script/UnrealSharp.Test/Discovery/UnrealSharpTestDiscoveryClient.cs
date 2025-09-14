using System.Collections;
using System.Reflection;
using System.Reflection.Metadata;
using NUnit.Framework;
using NUnit.Framework.Interfaces;
using NUnit.Framework.Internal;
using UnrealSharp.Core;
using UnrealSharp.Test.Attributes;
using UnrealSharp.Test.Interop;
using UnrealSharp.Test.Mappers;
using UnrealSharp.Test.Model;
using UnrealSharp.Test.Utils;

namespace UnrealSharp.Test.Discovery;

public static class UnrealSharpTestDiscoveryClient
{
    public static IEnumerable<UnrealTestMethod> DiscoverTests(ReadOnlySpan<FName> assemblyPaths)
    {
        var testCases = new List<UnrealTestMethod>();
        foreach (var assemblyName in assemblyPaths)
        {
            try
            {
                var assemblyPtr = ManagedTestingExporter.CallFindUserAssembly(assemblyName);
                var assembly = GCHandleUtilities.GetObjectFromHandlePtr<Assembly>(assemblyPtr);
                if (assembly is null)
                {
                    LogUnrealSharpTest.LogError($"Assembly {assemblyName} not found");
                    continue;
                }

                if (assembly.GetCustomAttribute<TestAssemblyAttribute>() is null)
                {
                    continue;
                }

                foreach (var type in assembly.GetTypes().Where(IsTestClass))
                {
                    try
                    {
                        DiscoverTests(testCases, assemblyName, type);
                    }
                    catch (Exception e)
                    {
                        LogUnrealSharpTest.LogError($"Failed to load test class {type}: {e}");
                    }
                }
            }
            catch (Exception e)
            {
                LogUnrealSharpTest.LogError($"Failed to load assembly {assemblyName}: {e}");
            }
        }

        return testCases;
    }

    private static bool IsTestClass(Type type)
    {
        if (!type.IsClass)
        {
            return false;
        }

        return type.GetCustomAttribute<TestFixtureAttribute>() is not null
            || type.GetMethods().Any(IsTestMethod);
    }

    private static bool IsTestMethod(MethodInfo method)
    {
        return method.GetCustomAttribute<TestAttribute>() is not null
            || method.GetCustomAttributes<TestCaseAttribute>().Any()
            || method.GetCustomAttributes<TestCaseSourceAttribute>().Any()
            || method.GetCustomAttributes<CombinatorialAttribute>().Any()
            || method.GetCustomAttributes<SequentialAttribute>().Any()
            || method.GetCustomAttributes<PairwiseAttribute>().Any();
    }

    private static void DiscoverTests(
        List<UnrealTestMethod> testCases,
        FName assemblyName,
        Type testClass
    )
    {
        var category = testClass.GetCustomAttribute<TestFixtureAttribute>()?.Category;
        var prefix = category ?? testClass.FullName ?? testClass.Name;

        MethodInfo? setupMethod = null;
        MethodInfo? teardownMethod = null;
        foreach (var method in testClass.GetMethods())
        {
            if (method.GetCustomAttribute<SetUpAttribute>() is not null)
            {
                if (setupMethod is not null)
                    throw new InvalidOperationException("Only one setup method is allowed");
                setupMethod = method;
            }

            if (method.GetCustomAttribute<TearDownAttribute>() is null)
                continue;

            if (teardownMethod is not null)
                throw new InvalidOperationException("Only one teardown method is allowed");
            teardownMethod = method;
        }

        foreach (var method in testClass.GetMethods().Where(IsTestMethod))
        {
            GetTestCases(testCases, assemblyName, prefix, method, setupMethod, teardownMethod);
        }
    }

    private static void GetTestCases(
        List<UnrealTestMethod> testCases,
        FName assemblyName,
        string prefix,
        MethodInfo method,
        MethodInfo? setupMethod,
        MethodInfo? teardownMethod
    )
    {
        var displayName = method.GetCustomAttribute<TestAttribute>()?.Description;
        var testName = displayName is not null
            ? $"{prefix}.{displayName}"
            : $"{prefix}.{method.Name}";

        var sequencePoint = method.GetFirstSequencePoint();

        var testCaseSourceAttributes = method
            .GetCustomAttributes<TestCaseSourceAttribute>(false)
            .ToArray();
        if (testCaseSourceAttributes.Length > 0)
        {
            GenerateTestCasesFromSource(
                testCases,
                assemblyName,
                method,
                setupMethod,
                teardownMethod,
                testCaseSourceAttributes,
                testName,
                sequencePoint
            );
            return;
        }

        var testCasesAttributes = method.GetCustomAttributes<TestCaseAttribute>(false).ToArray();

        if (testCasesAttributes.Length > 0)
        {
            CreateTestCases(
                testCases,
                assemblyName,
                method,
                setupMethod,
                teardownMethod,
                testCasesAttributes,
                testName,
                sequencePoint
            );

            return;
        }

        CreateTestCaseFromMethod(
            testCases,
            assemblyName,
            method,
            setupMethod,
            teardownMethod,
            testName,
            sequencePoint
        );
    }

    private static void CreateTestCaseFromMethod(
        List<UnrealTestMethod> testCases,
        FName assemblyName,
        MethodInfo method,
        MethodInfo? setupMethod,
        MethodInfo? teardownMethod,
        string testName,
        SequencePoint? sequencePoint
    )
    {
        try
        {
            var methodWrapper = new MethodWrapper(method.DeclaringType!, method);
            testCases.Add(
                CreateTestCase(
                    assemblyName,
                    testName,
                    method,
                    setupMethod,
                    teardownMethod,
                    sequencePoint,
                    GetPossibleValues(methodWrapper.GetParameters(), method.GetCombinationMode())
                )
            );
        }
        catch (Exception e)
        {
            LogUnrealSharpTest.LogError($"Failed to create test case {testName}: {e}");
        }
    }

    private static CombinationMode GetCombinationMode(this MethodInfo methodInfo)
    {
        var sequentialAttribute = methodInfo.GetCustomAttribute<SequentialAttribute>();
        if (sequentialAttribute is not null)
        {
            return CombinationMode.Sequential;
        }

        var pairwiseAttribute = methodInfo.GetCustomAttribute<PairwiseAttribute>();
        if (pairwiseAttribute is not null)
        {
            return CombinationMode.Pairwise;
        }

        return CombinationMode.Combinatorial;
    }

    private static IEnumerable<TestCaseData> GetPossibleValues(
        IEnumerable<IParameterInfo> parameters,
        CombinationMode combinationMode
    )
    {
        var parameterSources = parameters.Select(GetPossibleValues).ToArray();

        if (parameterSources.Length == 0)
            return [];

        var result = combinationMode switch
        {
            CombinationMode.Combinatorial => parameterSources.CartesianProduct(),
            CombinationMode.Sequential => parameterSources.SequentialGrouping(),
            CombinationMode.Pairwise => parameterSources.PairwiseGrouping(),
            _ => throw new ArgumentOutOfRangeException(
                nameof(combinationMode),
                combinationMode,
                null
            ),
        };

        return result.Select(x => x.ToArray()).Select(x => new TestCaseData(x));
    }

    private static object?[] GetPossibleValues(IParameterInfo parameter)
    {
        var allParameterOptions = parameter
            .ParameterInfo.GetCustomAttributes<NUnitAttribute>()
            .SelectMany(a => GetPossibleValues(parameter, a))
            .Distinct()
            .ToArray();

        if (allParameterOptions.Length > 0)
        {
            return allParameterOptions;
        }

        return parameter.ParameterType.IsValueType
            ? [Activator.CreateInstance(parameter.ParameterType)]
            : [null];
    }

    private static IEnumerable<object?> GetPossibleValues(
        IParameterInfo parameter,
        NUnitAttribute attribute
    )
    {
        return attribute switch
        {
            ValuesAttribute valuesAttribute => valuesAttribute.GetData(parameter).Cast<object>(),
            RangeAttribute rangeAttribute => rangeAttribute.GetData(parameter).Cast<object>(),
            ValueSourceAttribute valueSourceAttribute => valueSourceAttribute
                .GetData(parameter)
                .Cast<object>(),
            DynamicRandomAttribute randomAttribute => Enumerable
                .Range(0, randomAttribute.Count)
                .Select(i => new RandomPlaceholder(randomAttribute, parameter.ParameterInfo, i)),
            _ => [],
        };
    }

    private static void CreateTestCases(
        List<UnrealTestMethod> testCases,
        FName assemblyName,
        MethodInfo method,
        MethodInfo? setupMethod,
        MethodInfo? teardownMethod,
        TestCaseAttribute[] testCasesAttributes,
        string testName,
        SequencePoint? sequencePoint
    )
    {
        try
        {
            testCases.Add(
                CreateTestCase(
                    assemblyName,
                    testName,
                    method,
                    setupMethod,
                    teardownMethod,
                    sequencePoint,
                    testCasesAttributes.Select(t => t.ToTestCaseData())
                )
            );
        }
        catch (Exception e)
        {
            LogUnrealSharpTest.LogError($"Failed to create test case {testName}: {e}");
        }
    }

    private static void GenerateTestCasesFromSource(
        List<UnrealTestMethod> testCases,
        FName assemblyName,
        MethodInfo method,
        MethodInfo? setupMethod,
        MethodInfo? teardownMethod,
        TestCaseSourceAttribute[] testCaseSourceAttributes,
        string testName,
        SequencePoint? sequencePoint
    )
    {
        foreach (var sourceAttribute in testCaseSourceAttributes)
        {
            try
            {
                testCases.Add(
                    CreateTestCase(
                        assemblyName,
                        testName,
                        method,
                        setupMethod,
                        teardownMethod,
                        sequencePoint,
                        GetTestCaseSourceArguments(method, sourceAttribute)
                    )
                );
            }
            catch (Exception e)
            {
                LogUnrealSharpTest.LogError(
                    $"Failed to create test cases from source for {testName}: {e}"
                );
            }
        }
    }

    private static UnrealTestMethod CreateTestCase(
        FName assemblyName,
        string testName,
        MethodInfo method,
        MethodInfo? setupMethod,
        MethodInfo? teardownMethod,
        SequencePoint? sequencePoint,
        IEnumerable<TestCaseData>? testCases = null
    )
    {
        return new UnrealTestMethod(assemblyName, testName, setupMethod, teardownMethod, method)
        {
            TestCases =
                testCases
                    ?.Select((t, i) => (TestCase: t, Name: new FName($"TestCase{i + 1}")))
                    .ToOrderedDictionary(x => x.Name, x => x.TestCase)
                ?? new OrderedDictionary<FName, TestCaseData>(),
            CodeFilePath = sequencePoint?.Document.ToString(),
            LineNumber = sequencePoint?.StartLine ?? 0,
        };
    }

    private static IEnumerable<TestCaseData> GetTestCaseSourceArguments(
        MethodInfo method,
        TestCaseSourceAttribute sourceAttribute
    )
    {
        var sourceType = sourceAttribute.SourceType ?? method.DeclaringType!;
        var sourceName = sourceAttribute.SourceName;

        foreach (
            var member in sourceType.GetMember(
                sourceName!,
                BindingFlags.Static | BindingFlags.Public | BindingFlags.NonPublic
            )
        )
        {
            switch (member)
            {
                case FieldInfo fieldInfo:
                    {
                        var result = fieldInfo.GetValue(null);
                        if (ConvertTestCaseResult(result, out var objectsEnumerable))
                            return objectsEnumerable;
                    }
                    break;
                case PropertyInfo propertyInfo:
                    {
                        var result = propertyInfo.GetValue(null);
                        if (ConvertTestCaseResult(result, out var objectsEnumerable))
                            return objectsEnumerable;
                    }
                    break;
                case MethodInfo methodInfo:
                    {
                        var result = methodInfo.Invoke(null, null);
                        if (ConvertTestCaseResult(result, out var objectsEnumerable))
                            return objectsEnumerable;
                    }
                    break;
            }
        }

        throw new InvalidOperationException(
            $"Could not find TestCaseSource {sourceName} on {sourceType.FullName}"
        );
    }

    private static bool ConvertTestCaseResult(
        object? result,
        out IEnumerable<TestCaseData> objectsEnumerable
    )
    {
        switch (result)
        {
            case IEnumerable<object?[]> arrayData:
                objectsEnumerable = arrayData.Select(a => new TestCaseData(a));
                return true;
            case IEnumerable<TestCaseData> testCaseData:
                objectsEnumerable = testCaseData;
                return true;
            case IEnumerable enumerable:
                objectsEnumerable = enumerable.Cast<object>().Select(o => new TestCaseData(o));
                return true;
        }

        objectsEnumerable = [];
        return false;
    }
}
