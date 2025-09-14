using System.Reflection;
using NUnit.Framework;

namespace UnrealSharp.Test.Model;

/// <summary>
/// Represents a test method belonging to a specific assembly within the Unreal Engine testing framework.
/// </summary>
/// <remarks>
/// This class is a sealed record, ensuring immutability, and is purpose-built to integrate with Unreal Engine's
/// testing infrastructure. It includes metadata and configuration for executing the specified test case.
/// </remarks>
/// <param name="AssemblyName">
/// The name of the assembly in which the test method resides.
/// </param>
/// <param name="FullyQualifiedName">
/// The fully qualified name of the test method, including its namespace and class.
/// </param>
/// <param name="SetupMethod">
/// Optional setup method invoked before the test executes. Can be null if a setup method is not defined.
/// </param>
/// <param name="TearDownMethod">
/// Optional teardown method invoked after the test execution completes. Can be null if a teardown method is not defined.
/// </param>
/// <param name="Method">
/// The <see cref="MethodInfo"/> representing the test method being executed.
/// </param>
/// <remarks>
/// The <see cref="UnrealTestMethod"/> class also supports additional metadata, including the file path and
/// line number for code navigation, as well as a collection of <see cref="TestCaseData"/> representing available test cases.
/// </remarks>
public sealed record UnrealTestMethod(
    FName AssemblyName,
    string FullyQualifiedName,
    MethodInfo? SetupMethod,
    MethodInfo? TearDownMethod,
    MethodInfo Method
)
{
    /// <summary>
    /// Represents a collection of test cases associated with a specific test method.
    /// </summary>
    /// <remarks>
    /// Each test case is uniquely identified by a key of type <c>FName</c> and is associated
    /// with a <c>TestCaseData</c> object. It is typically used to supply input data and expected
    /// outcomes for the test method execution.
    /// </remarks>
    /// <value>
    /// An ordered dictionary where the keys are of type <c>FName</c> and the values
    /// are <c>TestCaseData</c> instances. By default, it is initialized as an empty ordered
    /// dictionary.
    /// </value>
    public IReadOnlyDictionary<FName, TestCaseData> TestCases { get; init; } =
        new OrderedDictionary<FName, TestCaseData>();

    /// <summary>
    /// Represents the file path of the source code file where the associated method is defined.
    /// </summary>
    /// <remarks>
    /// This property is used to provide the location of the source file containing the method,
    /// derived from debugging symbols or other metadata during test discovery.
    /// </remarks>
    /// <value>
    /// A string containing the file path to the source code file, or <c>null</c> if the file path
    /// could not be determined.
    /// </value>
    public string? CodeFilePath { get; init; }

    /// <summary>
    /// Represents the line number in the source code where the method is defined.
    /// </summary>
    /// <remarks>
    /// This property is typically used to point to the start line of the test method's implementation
    /// in the corresponding source file. It is particularly useful for debugging or reporting purposes.
    /// </remarks>
    /// <value>
    /// An integer representing the line number of the method in the source file. Defaults to 0 if the line number
    /// information is unavailable.
    /// </value>
    public int LineNumber { get; init; }
}
