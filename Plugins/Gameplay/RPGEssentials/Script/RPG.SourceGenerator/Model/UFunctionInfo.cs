using System.Collections.Immutable;
using JetBrains.Annotations;
using Microsoft.CodeAnalysis;

namespace RPG.SourceGenerator.Model;

[UsedImplicitly]
public record UFunctionInfo(
    string Name,
    ITypeSymbol ReturnType,
    ImmutableArray<AttributeInfo> Attributes,
    ImmutableArray<UParamInfo> Parameters
)
{
    [UsedImplicitly]
    public bool ReturnsVoid => ReturnType.SpecialType == SpecialType.System_Void;

    [UsedImplicitly]
    public bool HasParameters => Parameters.Length > 0;

    [UsedImplicitly]
    public string? OptionType =>
        ReturnType is INamedTypeSymbol { IsGenericType: true, MetadataName: "TOptional`1" } optionType
            ? optionType.TypeArguments[0].ToDisplayString()
            : null;

    [UsedImplicitly]
    public string? NullableType =>
        ReturnType is INamedTypeSymbol { IsGenericType: true, MetadataName: "Nullable`1" } optionType
            ? optionType.TypeArguments[0].ToDisplayString()
            : null;

    [UsedImplicitly]
    public bool ReturnsOption => OptionType is not null;

    [UsedImplicitly]
    public bool ReturnsNullable => NullableType is not null;

    [UsedImplicitly]
    public bool ReturnsOptionOrNullable => ReturnsOption || ReturnsNullable;

    [UsedImplicitly]
    public string UnderlyingType => OptionType ?? NullableType ?? ReturnType.ToDisplayString();

    public required bool IsExposed { get; init; }

    public required string? GenericConstraint { get; init; }
}
