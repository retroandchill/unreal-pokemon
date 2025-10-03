namespace Pokemon.Editor.Serialization.Pbs.Attributes;

/// <summary>
/// An attribute used to define localization metadata for text properties or parameters in PBS-related models.
/// </summary>
/// <remarks>
/// This attribute is applied to properties or parameters that are intended to be localized.
/// It specifies the namespace and the key format to retrieve the localized text.
/// The namespace represents a logical grouping of localized strings (e.g., abilities, moves),
/// while the key format determines the specific key pattern for accessing the localized value in the corresponding resource.
/// </remarks>
/// <param name="ns">The namespace for the localized text.</param>
/// <param name="keyFormat">The key format for the localized text.</param>
[AttributeUsage(AttributeTargets.Property | AttributeTargets.Parameter)]
public class PbsLocalizedTextAttribute(string ns, string keyFormat) : Attribute
{
    /// <summary>
    /// Gets the namespace associated with the localized text.
    /// </summary>
    /// <remarks>
    /// The namespace represents a logical grouping for localized strings, such as abilities, moves,
    /// or other categorical divisions. It defines the scope or category under which the localization
    /// operation is performed.
    /// </remarks>
    public string Namespace { get; } = ns;

    /// <summary>
    /// Gets the key format associated with the localized text.
    /// </summary>
    /// <remarks>
    /// The key format defines the structure or pattern used to access the localized text within the specified namespace.
    /// It serves as a template for generating the keys that map to specific localized string entries.
    /// This is particularly useful for ensuring consistent key generation across various localized resources.
    /// </remarks>
    public string KeyFormat { get; } = keyFormat;
}
