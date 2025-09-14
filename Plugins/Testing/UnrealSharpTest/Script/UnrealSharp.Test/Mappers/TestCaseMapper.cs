using NUnit.Framework;
using Riok.Mapperly.Abstractions;
using UnrealSharp.Test.Model;
using UnrealSharp.UnrealSharpTest;

namespace UnrealSharp.Test.Mappers;

/// <summary>
/// Provides mappings for converting and adapting test-related models within the UnrealSharp testing framework.
/// </summary>
[Mapper(
    RequiredMappingStrategy = RequiredMappingStrategy.Target,
    PreferParameterlessConstructors = false
)]
public static partial class TestCaseMapper
{
    /// <summary>
    /// Converts an instance of <see cref="UnrealTestMethod"/> to an instance of <see cref="FManagedTestCase"/>.
    /// </summary>
    /// <param name="testMethod">The <see cref="UnrealTestMethod"/> instance to be converted.</param>
    /// <returns>A new instance of <see cref="FManagedTestCase"/> containing the mapped data from the provided <see cref="UnrealTestMethod"/>.</returns>
    public static partial FManagedTestCase ToManagedTestCase(this UnrealTestMethod testMethod);

    /// <summary>
    /// Converts an instance of <see cref="TestCaseAttribute"/> to an instance of <see cref="TestCaseData"/>.
    /// </summary>
    /// <param name="testCaseAttribute">The <see cref="TestCaseAttribute"/> instance to be converted.</param>
    /// <returns>A new instance of <see cref="TestCaseData"/> containing the data mapped from the provided <see cref="TestCaseAttribute"/>.</returns>
    [MapperIgnoreTarget(nameof(TestCaseAttribute.Arguments))]
    public static partial TestCaseData ToTestCaseData(this TestCaseAttribute testCaseAttribute);

    /// <summary>
    /// Creates a new instance of <see cref="TestCaseData"/> using the provided <see cref="TestCaseAttribute"/>.
    /// </summary>
    /// <param name="testCaseAttribute">The <see cref="TestCaseAttribute"/> that contains the arguments for the test case.</param>
    /// <returns>A new instance of <see cref="TestCaseData"/> initialized with the arguments from the specified <see cref="TestCaseAttribute"/>.</returns>
    [ObjectFactory]
    private static TestCaseData CreateTestCaseData(TestCaseAttribute testCaseAttribute)
    {
        return new TestCaseData(testCaseAttribute.Arguments);
    }
}
