using JetBrains.Annotations;
using Microsoft.CodeAnalysis;

namespace RPG.SourceGenerator.Model;

public record UParamInfo(
    IParameterSymbol Parameter,
    string RefQualifier = "",
    string? DefaultValue = null,
    bool IsLast = false
)
{
    public ITypeSymbol Type => Parameter.Type;
    public string Name => Parameter.Name;
    
    public bool HasDefaultValue => DefaultValue is not null;
}
