namespace Pokemon.Editor.Serialization.Pbs.Attributes;

[AttributeUsage(AttributeTargets.Property | AttributeTargets.Parameter)]
public class PbsLocalizedTextAttribute(string ns, string keyFormat) : Attribute
{
    public string Namespace { get; } = ns;
    public string KeyFormat { get; } = keyFormat;
}
