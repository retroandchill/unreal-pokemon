using GameAccessTools.SourceGenerator.Attributes;
using Retro.SourceGeneratorUtilities.Utilities.Attributes;

namespace GameAccessTools.SourceGenerator.Model;

[AttributeInfoType<GameDataProviderAttribute>]
public readonly record struct GameDataProviderInfo
{
    public string? DisplayName { get; init; }
    public string Category { get; init; }
}
