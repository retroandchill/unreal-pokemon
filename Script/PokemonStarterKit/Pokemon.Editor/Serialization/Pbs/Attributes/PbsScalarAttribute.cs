using Pokemon.Editor.Serialization.Pbs.Converters;

namespace Pokemon.Editor.Serialization.Pbs.Attributes;

public abstract class PbsScalarAttribute : Attribute
{
    public abstract Type ConverterType { get; }
}

[AttributeUsage(
    AttributeTargets.Property
        | AttributeTargets.Parameter
        | AttributeTargets.Class
        | AttributeTargets.Struct
        | AttributeTargets.Enum
        | AttributeTargets.Interface
)]
public sealed class PbsScalarAttribute<T> : PbsScalarAttribute
    where T : IPbsConverter
{
    public override Type ConverterType => typeof(T);
}
