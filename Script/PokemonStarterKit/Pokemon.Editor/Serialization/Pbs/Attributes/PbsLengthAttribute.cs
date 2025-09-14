namespace Pokemon.Editor.Serialization.Pbs.Attributes;

[AttributeUsage(AttributeTargets.Property)]
public class PbsLengthAttribute : Attribute
{
    public int Min { get; init; } = 0;
    public int Max { get; init; } = int.MaxValue;
}
