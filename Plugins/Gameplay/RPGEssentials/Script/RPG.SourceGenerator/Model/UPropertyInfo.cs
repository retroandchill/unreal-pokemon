using System.Collections.Immutable;
using System.Diagnostics.CodeAnalysis;
using JetBrains.Annotations;
using Microsoft.CodeAnalysis;

namespace RPG.SourceGenerator.Model;

public readonly record struct AccessorInfo(bool IsUFunction, ImmutableArray<AttributeInfo> Attributes);

public record UPropertyInfo(
    IPropertySymbol Property,
    AccessorInfo? Getter,
    AccessorInfo? Setter,
    string? DisplayName,
    string? Category
)
{
    public ITypeSymbol Type => Property.Type;
    public string Name => Property.Name;
    
    public bool IsOptionOrNullableType => IsOptionType || IsNullableType;

    public bool IsOptionType => OptionType is not null;

    public ITypeSymbol? OptionType =>
        Type
            is INamedTypeSymbol
            {
                IsGenericType: true,
                ContainingNamespace.Name: "UnrealSharp",
                MetadataName: "TOptional`1"
            } optionType
            ? optionType.TypeArguments[0]
            : null;

    public bool IsNullableType => NullableType is not null;

    public ITypeSymbol? NullableType =>
        Type is INamedTypeSymbol { IsGenericType: true, MetadataName: "Nullable`1" } optionType
            ? optionType.TypeArguments[0]
            : null;

    public ITypeSymbol UnderlyingType => OptionType ?? NullableType ?? Type;

    public bool HasGetter => Getter.HasValue;

    public bool HasSetter => Setter.HasValue;

    public bool GetterIsUFunction => Getter?.IsUFunction ?? false;

    public ImmutableArray<AttributeInfo> GetterAttributes => Getter?.Attributes ?? [];

    public bool SetterIsUFunction => Setter?.IsUFunction ?? false;

    public ImmutableArray<AttributeInfo> SetterAttributes => Setter?.Attributes ?? [];

    [MemberNotNullWhen(true, nameof(DisplayName))]
    public bool HasDisplayName => DisplayName is not null;

    [MemberNotNullWhen(true, nameof(Category))]
    public bool HasCategory => Category is not null;

    public required bool IsExposed { get; init; }
}
