#if GAME_DATA_ACCESS_TOOLS_GENERATOR
using RhoMicro.CodeAnalysis;
#endif

namespace GameAccessTools.SourceGenerator.Attributes;

/// <summary>
/// Represents an attribute that specifies a singular name for the targeted property.
/// This can be utilized to provide a custom singular name definition for a property,
/// often in contexts where properties are processed or represented programmatically.
/// </summary>
/// <remarks>
/// The <see cref="SingularNameAttribute"/> is primarily intended for use with code generation
/// and source analysis utilities. It allows for defining and maintaining consistency of
/// singular naming conventions across elements.
/// </remarks>
/// <param name="name">The singular name associated with the marked property.</param>
/// <example>
/// This attribute may be applied to properties to specify a singular name for processing purposes.
/// Example scenarios include usage in domain-specific models or templates where a singular name
/// representation is required.
/// </example>
[AttributeUsage(AttributeTargets.Property)]
#if GAME_DATA_ACCESS_TOOLS_GENERATOR
[IncludeFile]
#endif
internal class SingularNameAttribute(string name) : Attribute
{
    public string Name { get; } = name;
}
