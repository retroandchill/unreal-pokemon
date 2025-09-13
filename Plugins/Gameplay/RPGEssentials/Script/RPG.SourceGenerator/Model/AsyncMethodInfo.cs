using System.Collections.Immutable;
using Microsoft.CodeAnalysis;

namespace RPG.SourceGenerator.Model;

public record AsyncMethodInfo(
    string Namespace,
    string ClassName,
    string MethodName,
    ITypeSymbol TaskType,
    ITypeSymbol ComponentClass,
    ImmutableArray<UParamInfo> Parameters,
    ImmutableArray<AttributeInfo> Attributes
)
{
    public ITypeSymbol? ReturnType =>
        TaskType is INamedTypeSymbol { IsGenericType: true } genericTaskType
            ? genericTaskType.TypeArguments[0]
            : null;
    public bool HasReturnType => TaskType is INamedTypeSymbol { IsGenericType: true };
    public bool ReturnsValueTask => TaskType.MetadataName is "ValueTask" or "ValueTask`1";

    public bool HasParameters => Parameters.Length > 0;
}
