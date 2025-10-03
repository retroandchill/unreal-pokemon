#if RPG_ESSENTIALS_GENERATOR
using RhoMicro.CodeAnalysis;
#endif

namespace RPG.SourceGenerator.Attributes;

/// <summary>
/// Specifies that a property requires initialization by referencing the provided initializer.
/// </summary>
/// <remarks>
/// This attribute is intended for use on properties to indicate that an associated initializer must
/// be used for assigning its value. It provides a mechanism for connecting a property to a named
/// initialization logic for code generation purposes.
/// </remarks>
/// <param name="initializerName">
/// The name of the initializer associated with the property. This name should correspond to
/// the specific initialization logic to be applied.
/// </param>
/// <example>
/// Intended for use in scenarios where properties require specific initialization patterns
/// to facilitate consistent code generation.
/// </example>
#if RPG_ESSENTIALS_GENERATOR
[IncludeFile]
#endif
[AttributeUsage(AttributeTargets.Property)]
internal class InitializerAttribute(string initializerName) : Attribute
{
    /// <summary>
    /// Gets the name of the initializer associated with the property.
    /// </summary>
    /// <remarks>
    /// The initializer name specifies the initialization logic that must be applied to the property.
    /// This is used in code generation to ensure the property is initialized consistently with
    /// the desired behavior.
    /// </remarks>
    public string InitializerName { get; } = initializerName;
}
