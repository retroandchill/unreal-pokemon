using Microsoft.CodeAnalysis;

namespace GameAccessTools.SourceGenerator.Model;

public enum MarshallerType
{
    Regular,
    Instanced,
    View,
}

public readonly record struct MarshallerInfo(
    string Name,
    MarshallerType Type = MarshallerType.Regular,
    string? ChildMarshallerType = null,
    string? ValueMarshallerType = null
)
{
    public MarshallerInfo(
        string name,
        string? ChildMarshallerType,
        string? ValueMarshallerType = null
    )
        : this(name, MarshallerType.Instanced, ChildMarshallerType, ValueMarshallerType) { }

    public bool IsInstanced => Type != MarshallerType.Regular && ChildMarshallerType is not null;

    public bool IsView => Type == MarshallerType.View && ChildMarshallerType is not null;

    public bool HasValue => Type != MarshallerType.Regular && ValueMarshallerType is not null;
}

public readonly record struct MarshalledPropertyInfo(
    string MarshalledType,
    MarshallerInfo MarshallerInfo
);

public record AccessiblePropertyInfo(string Access, string Type, string Name)
{
    public required MarshallerInfo MarshallerInfo { get; init; }

    public bool MarshallerInstanced => MarshallerInfo.IsInstanced;

    public bool MarshallerIsView => MarshallerInfo.IsView;

    public bool MarshallerHasValue => MarshallerInfo.HasValue;

    public string? GetterBody { get; init; }

    public bool HasGetterBody => GetterBody is not null;

    public bool HasBackingField { get; init; } = true;

    public bool IsExpressionBody { get; init; } = false;
}

public record TranslatedMethod(
    string Modifiers,
    string ReturnType,
    string Name,
    string? TypeParams,
    string ParameterList,
    string Constraints,
    string Body,
    bool IsExpressionBody = false
);
