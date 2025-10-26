using System.Collections.Immutable;
using JetBrains.Annotations;
using Microsoft.CodeAnalysis;

namespace RPG.SourceGenerator.Model;

public record UFunctionInfo(
    string Name,
    ITypeSymbol ReturnType,
    ImmutableArray<AttributeInfo> Attributes,
    ImmutableArray<UParamInfo> Parameters
)
{
    public bool ReturnsVoid => ReturnType.SpecialType == SpecialType.System_Void;

    public bool HasParameters => Parameters.Length > 0;

    public string? OptionType =>
        ReturnType is INamedTypeSymbol { IsGenericType: true, MetadataName: "TOptional`1" } optionType
            ? optionType.TypeArguments[0].ToDisplayString()
            : null;

    public string? NullableType =>
        ReturnType is INamedTypeSymbol { IsGenericType: true, MetadataName: "Nullable`1" } optionType
            ? optionType.TypeArguments[0].ToDisplayString()
            : null;

    public bool ReturnsOption => OptionType is not null;

    public bool ReturnsNullable => NullableType is not null;

    public bool ReturnsOptionOrNullable => ReturnsOption || ReturnsNullable;

    public string UnderlyingType => OptionType ?? NullableType ?? ReturnType.ToDisplayString();

    public required bool IsExposed { get; init; }

    public required string? GenericConstraint { get; init; }
}
