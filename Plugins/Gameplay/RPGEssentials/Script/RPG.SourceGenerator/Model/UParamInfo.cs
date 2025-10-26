using JetBrains.Annotations;
using Microsoft.CodeAnalysis;

namespace RPG.SourceGenerator.Model;

public record UParamInfo(
    ITypeSymbol Type,
    string Name,
    string RefQualifier = "",
    string? DefaultValue = null,
    bool IsLast = false
)
{
    public bool HasDefaultValue => DefaultValue is not null;
}
