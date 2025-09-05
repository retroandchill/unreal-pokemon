using GameAccessTools.SourceGenerator.Attributes;
using Retro.SourceGeneratorUtilities.Utilities.Attributes;

namespace GameAccessTools.SourceGenerator.Model;

[AttributeInfoType<GameDataEntryAttribute>]
public readonly record struct GameDataEntryInfo
{
    public string? GeneratedClassName { get; init; }
}
