using Retro.SourceGeneratorUtilities.Utilities.Attributes;
using RPG.SourceGenerator.Attributes;

namespace RPG.SourceGenerator.Model;

[AttributeInfoType<FactoryAttribute>]
public readonly record struct FactoryInfo
{
    public string? SubclassSource { get; init; }
}
