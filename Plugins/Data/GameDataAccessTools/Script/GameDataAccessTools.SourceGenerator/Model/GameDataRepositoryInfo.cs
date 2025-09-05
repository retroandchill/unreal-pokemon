using Microsoft.CodeAnalysis;

namespace GameAccessTools.SourceGenerator.Model;

public readonly record struct GameDataRepositoryInfo(
    INamedTypeSymbol EntryType,
    string AssetClassName
)
{
    public INamespaceSymbol? Namespace => EntryType.ContainingNamespace;

    public string EntryName => EntryType.Name;
}
