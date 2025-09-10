using System.Numerics;

namespace Pokemon.Editor.Serialization.Pbs.Attributes;

[AttributeUsage(AttributeTargets.Property | AttributeTargets.Parameter)]
public class PbsRangeAttribute<T> : Attribute
    where T : struct, INumber<T>
{
    public PbsRangeAttribute(T min)
    {
        Min = min;
    }

    public PbsRangeAttribute(T min, T max)
    {
        Min = min;
        Max = max;
    }

    public PbsRangeAttribute(object? min, T max)
    {
        Min = min is T t ? t : default;
        Max = max;
    }

    public T? Min { get; }

    public T? Max { get; }
}
