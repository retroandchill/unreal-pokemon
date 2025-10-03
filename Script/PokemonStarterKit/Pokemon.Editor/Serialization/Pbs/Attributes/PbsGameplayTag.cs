namespace Pokemon.Editor.Serialization.Pbs.Attributes;

/// <summary>
/// Specifies that a property or parameter represents a gameplay tag within the PBS (Pokémon Battle Simulator) system.
/// </summary>
/// <remarks>
/// The <see cref="PbsGameplayTag"/> attribute is used to annotate properties or parameters that should be serialized
/// and categorized as gameplay tags in the PBS framework. This helps organize, classify, and dynamically manage
/// gameplay metadata using the Gameplay Tag system.
/// The attribute supports configuring a namespace, optionally enabling the creation of missing gameplay tags,
/// and specifying a custom separator for tag hierarchy.
/// The namespace parameter defines the logical group or category of the gameplay tag. Set the <c>Create</c> property
/// to <c>true</c> if you wish to allow automatic creation of tags during processing. Additionally, the
/// <c>Separator</c> property can be used to define a delimiter that separates parts of the tag hierarchy.
/// This attribute is primarily employed in serialization scenarios where gameplay tags are automatically
/// generated or validated based on PBS data files.
/// </remarks>
/// <param name="ns">The namespace for the gameplay tag.</param>
/// <example>
/// The <see cref="PbsGameplayTag"/> attribute is typically applied to properties within record types
/// in PBS models where gameplay tags are used to classify metadata.
/// </example>
[AttributeUsage(AttributeTargets.Property | AttributeTargets.Parameter)]
public class PbsGameplayTag(string ns) : Attribute
{
    /// <summary>
    /// Gets the namespace associated with the Gameplay Tag.
    /// This property holds the string value that defines the namespace for the Gameplay Tag.
    /// The namespace is used to categorize or organize gameplay tags logically.
    /// </summary>
    public string Namespace { get; } = ns;

    /// <summary>
    /// Indicates whether new gameplay tags can be created in the associated namespace.
    /// This property is used to define if the functionality allows for tag creation
    /// when applying the PbsGameplayTag attribute.
    /// </summary>
    public bool Create { get; init; }

    /// <summary>
    /// Gets the separator string used to delimit parts of a Gameplay Tag.
    /// This property defines the character or string that serves as a separator
    /// within the structure of the tag, enabling hierarchical organization or customization.
    /// </summary>
    public string? Separator { get; init; }
}
