namespace UnrealSharp.Test.Attributes;

/// <summary>
/// Represents an attribute used to mark an assembly as containing test cases.
/// </summary>
/// <remarks>
/// This attribute is intended to indicate that the specified assembly includes test-related classes or methods.
/// It can be used for test discovery and filtering within the assembly.
/// </remarks>
/// <example>
/// Apply this attribute to an assembly to signal that it contains test cases.
/// </example>
/// <usage>
/// To utilize this attribute, decorate the appropriate assembly with it.
/// </usage>
[AttributeUsage(AttributeTargets.Assembly)]
public class TestAssemblyAttribute : Attribute;
