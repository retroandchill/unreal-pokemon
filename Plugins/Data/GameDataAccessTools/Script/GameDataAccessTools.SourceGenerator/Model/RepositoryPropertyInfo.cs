using GameAccessTools.SourceGenerator.Utilities;
using Microsoft.CodeAnalysis;

namespace GameAccessTools.SourceGenerator.Model;

public record RepositoryPropertyInfo
{
    public required string Type { get; init; }
    public required string Name { get; init; }
    public required string SingularName { get; init; }
    public required ITypeSymbol EntryType { get; init; }

    public string? Categories =>
        EntryType
            .GetMembers()
            .OfType<IPropertySymbol>()
            .Where(x => x.Name == "Id")
            .SelectMany(x => x.GetAttributes())
            .Where(x =>
                x.AttributeClass?.ToDisplayString() == SourceContextNames.UMetaDataAttribute
                && x.ConstructorArguments.Length == 2
                && x.ConstructorArguments[0].Value is "Categories"
                && x.ConstructorArguments[1].Value is string
            )
            .Select(x => x.ConstructorArguments[1].Value as string)
            .FirstOrDefault();

    public bool HasCategories => Categories is not null;
    public string EntryTypeEngineName => EntryType.Name[1..];
    public required string RepositoryClassName { get; init; }
    public required string? Category { get; init; }
    public bool HasCategory => Category is not null;
}
