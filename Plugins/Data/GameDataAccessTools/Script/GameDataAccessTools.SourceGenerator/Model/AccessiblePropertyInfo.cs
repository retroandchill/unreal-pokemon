using Microsoft.CodeAnalysis;

namespace GameAccessTools.SourceGenerator.Model;

public readonly record struct MarshallerInfo(
    string Name,
    string? ChildMarshallerType = null,
    string? ValueMarshallerType = null
)
{
    public bool IsInstanced => ChildMarshallerType is not null;

    public bool HasValue => ValueMarshallerType is not null;
}

public record AccessiblePropertyInfo(ITypeSymbol Type, string Name)
{
    public required MarshallerInfo MarshallerInfo { get; init; }

    public bool MarshallerInstanced => MarshallerInfo.IsInstanced;

    public bool MarshallerHasValue => MarshallerInfo.HasValue;
}
