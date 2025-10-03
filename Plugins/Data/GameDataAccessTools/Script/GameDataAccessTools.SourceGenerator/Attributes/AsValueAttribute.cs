#if GAME_DATA_ACCESS_TOOLS_GENERATOR
using RhoMicro.CodeAnalysis;
#endif

namespace GameAccessTools.SourceGenerator.Attributes;

/// <summary>
/// An attribute used to indicate that a property or field should be treated as a "value" for the purposes
/// of specific source generation logic. This may affect how properties or fields are accessed, converted, or marshaled
/// in generated code.
/// </summary>
/// <remarks>
/// This attribute can be applied to properties or fields only. Its presence may influence how the field or property
/// is processed within the source generation context, including but not limited to its representation and data handling.
/// </remarks>
/// <example>
/// This attribute can impact behavior such as conversion or marshalling when interacting with the property or field
/// during code generation. For instance, properties marked with this attribute may bypass certain default behaviors
/// like enumerating or wrapping collections.
/// </example>
[AttributeUsage(AttributeTargets.Property | AttributeTargets.Field)]
#if GAME_DATA_ACCESS_TOOLS_GENERATOR
[IncludeFile]
#endif
internal class AsValueAttribute : Attribute;
