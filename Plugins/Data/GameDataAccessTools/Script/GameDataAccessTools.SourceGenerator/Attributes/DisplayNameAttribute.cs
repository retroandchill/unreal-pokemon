#if GAME_DATA_ACCESS_TOOLS_GENERATOR
using RhoMicro.CodeAnalysis;
#endif

namespace GameAccessTools.SourceGenerator.Attributes;

/// <summary>
/// Specifies the the marked property should be used as the display name for the associated data.
/// </summary>
[AttributeUsage(AttributeTargets.Property)]
#if GAME_DATA_ACCESS_TOOLS_GENERATOR
[IncludeFile]
#endif
internal class DisplayNameAttribute : Attribute;
