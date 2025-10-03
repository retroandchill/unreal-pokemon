#if RPG_ESSENTIALS_GENERATOR
using RhoMicro.CodeAnalysis;
#endif

namespace RPG.SourceGenerator.Attributes;

/// <summary>
/// Specifies that the attributed method is a factory method.
/// A factory method is responsible for creating and returning an instance of a particular object or type.
/// </summary>
/// <remarks>
/// This attribute is designed to be applied to methods.
/// It can provide additional functionality or metadata related to code generation or dynamic behavior.
/// </remarks>
/// <example>
/// This attribute does not contain functionality directly linked to runtime execution.
/// Instead, it could be utilized in source generators, analyzers, or other metaprogramming tasks.
/// </example>
#if RPG_ESSENTIALS_GENERATOR
[IncludeFile]
#endif
[AttributeUsage(AttributeTargets.Method)]
public class FactoryAttribute : Attribute
{
    /// <summary>
    /// The name of the method that retrieves the subclass to initialize. Good for when the subclass is not known at compile time, such as being loaded from a config file.
    /// </summary>
    public string? SubclassSource { get; init; }
}
