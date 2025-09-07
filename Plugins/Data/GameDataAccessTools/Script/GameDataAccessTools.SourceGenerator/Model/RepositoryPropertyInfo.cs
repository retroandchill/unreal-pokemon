using GameAccessTools.SourceGenerator.Utilities;
using Microsoft.CodeAnalysis;

namespace GameAccessTools.SourceGenerator.Model;

public record RepositoryPropertyInfo
{
    public required string Type { get; init; }
    public required string Name { get; init; }
    public required string SingularName { get; init; }
    public required ITypeSymbol EntryType { get; init; }
    public required bool HasAsset { get; init; }

    public string EntryTypeEngineName => EntryType.Name[1..];
    public required string RepositoryClassName { get; init; }
}
