using System.Collections.Immutable;
using JetBrains.Annotations;
using Microsoft.CodeAnalysis;

namespace RPG.SourceGenerator.Model;

public readonly record struct AccessorInfo(
    bool IsUFunction,
    ImmutableArray<AttributeInfo> Attributes
);

[UsedImplicitly]
public record UPropertyInfo(
    ITypeSymbol Type,
    string Name,
    AccessorInfo? Getter,
    AccessorInfo? Setter,
    string? DisplayName,
    string? Category
)
{
    [UsedImplicitly]
    public bool IsOptionOrNullableType => IsOptionType || IsNullableType;

    [UsedImplicitly]
    public bool IsOptionType => OptionType is not null;

    [UsedImplicitly]
    public ITypeSymbol? OptionType =>
        Type
            is INamedTypeSymbol
            {
                IsGenericType: true,
                ContainingNamespace.Name: "LanguageExt",
                MetadataName: "Option`1"
            } optionType
            ? optionType.TypeArguments[0]
            : null;

    [UsedImplicitly]
    public bool IsNullableType => NullableType is not null;

    [UsedImplicitly]
    public ITypeSymbol? NullableType =>
        Type is INamedTypeSymbol { IsGenericType: true, MetadataName: "Nullable`1" } optionType
            ? optionType.TypeArguments[0]
            : null;

    [UsedImplicitly]
    public ITypeSymbol UnderlyingType => OptionType ?? NullableType ?? Type;

    [UsedImplicitly]
    public bool HasGetter => Getter.HasValue;

    [UsedImplicitly]
    public bool HasSetter => Setter.HasValue;

    [UsedImplicitly]
    public bool GetterIsUFunction => Getter?.IsUFunction ?? false;

    [UsedImplicitly]
    public ImmutableArray<AttributeInfo> GetterAttributes => Getter?.Attributes ?? [];

    [UsedImplicitly]
    public bool SetterIsUFunction => Setter?.IsUFunction ?? false;

    [UsedImplicitly]
    public ImmutableArray<AttributeInfo> SetterAttributes => Setter?.Attributes ?? [];

    [UsedImplicitly]
    public bool HasDisplayName = DisplayName is not null;

    [UsedImplicitly]
    public bool HasCategory = Category is not null;

    public required bool IsExposed { get; init; }
}
