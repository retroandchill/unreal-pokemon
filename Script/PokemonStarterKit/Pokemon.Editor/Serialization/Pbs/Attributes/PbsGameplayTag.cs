namespace Pokemon.Editor.Serialization.Pbs.Attributes;

[AttributeUsage(AttributeTargets.Property | AttributeTargets.Parameter)]
public class PbsGameplayTag(string ns) : Attribute
{
    public string Namespace { get; } = ns;

    public bool Create { get; init; }

    public string? Separator { get; init; }
}
