#if GAME_DATA_ACCESS_TOOLS_GENERATOR
using RhoMicro.CodeAnalysis;

#else
#nullable enable
#endif

namespace GameAccessTools.SourceGenerator.Attributes;

#if GAME_DATA_ACCESS_TOOLS_GENERATOR
[IncludeFile]
#endif
internal class GameDataEntryAttribute : Attribute
{
    /// <summary>
    /// The name of the generated class, without the "U" prefix.
    /// Defaults to &lt;ClassName>DataRepository (or &lt;ClassName>Repository
    /// if the class name already ends with Data).
    /// </summary>
    public string? GeneratedClassName { get; init; }
}
