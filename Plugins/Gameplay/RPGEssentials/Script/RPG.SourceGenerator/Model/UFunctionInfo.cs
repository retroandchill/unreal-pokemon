using System.Collections.Immutable;
using JetBrains.Annotations;
using Microsoft.CodeAnalysis;

namespace RPG.SourceGenerator.Model;

public record UFunctionInfo(
    string Name,
    IMethodSymbol Method,
    ImmutableArray<AttributeInfo> Attributes,
    ImmutableArray<UParamInfo> Parameters
)
{
    public ITypeSymbol ReturnType => Method.ReturnType;

    public bool ReturnsVoid => ReturnType.SpecialType == SpecialType.System_Void;

    public bool HasParameters => Parameters.Length > 0;

    public ITypeSymbol? OptionType =>
        ReturnType is INamedTypeSymbol { IsGenericType: true, MetadataName: "TOptional`1" } optionType
            ? optionType.TypeArguments[0]
            : null;

    public ITypeSymbol? NullableType =>
        ReturnType is INamedTypeSymbol { IsGenericType: true, MetadataName: "Nullable`1" } optionType
            ? optionType.TypeArguments[0]
            : null;

    public bool ReturnsOption => OptionType is not null;

    public bool ReturnsNullable => NullableType is not null;

    public bool ReturnsOptionOrNullable => ReturnsOption || ReturnsNullable;

    public ITypeSymbol UnderlyingType => OptionType ?? NullableType ?? ReturnType;

    public required bool IsExposed { get; init; }

    public required string? GenericConstraint { get; init; }
}
