using Pokemon.Editor.Serialization.Pbs.Attributes;
using UnrealSharp;
using UnrealSharp.Core;
using UnrealSharp.GameplayTags;

namespace Pokemon.Editor.Serialization.Model.Pbs;

/// <summary>
/// Represents detailed information about a Pokemon type in the data serialization model.
/// </summary>
/// <remarks>
/// This record serves as a structured data model that defines various properties
/// of a Pokemon type, such as its name, weaknesses, resistances, immunities, and
/// additional metadata. It also incorporates localization support and gameplay tags
/// for extensibility and categorization.
/// </remarks>
/// <seealso cref="Pokemon.Editor.Serialization.Pbs.Attributes.PbsKeyAttribute"/>
/// <seealso cref="Pokemon.Editor.Serialization.Pbs.Attributes.PbsIndexAttribute"/>
/// <seealso cref="Pokemon.Editor.Serialization.Pbs.Attributes.PbsLocalizedTextAttribute"/>
/// <seealso cref="Pokemon.Editor.Serialization.Pbs.Attributes.PbsNameAttribute"/>
public record TypeInfo
{
    /// <summary>
    /// Gets the unique identifier for the Type information. This property is marked as a primary key
    /// for serialization purposes using the <see cref="PbsKeyAttribute"/>.
    /// </summary>
    [PbsKey]
    public FName ID { get; init; }

    /// <summary>
    /// Represents the index of the row within the serialized PBS data.
    /// This property is annotated with the <see cref="PbsIndexAttribute"/> to indicate its role
    /// in defining positional or indexed data for the associated type.
    /// </summary>
    [PbsIndex]
    public int RowIndex { get; init; }

    /// <summary>
    /// Gets the localized name for the type. The value is resolved using the
    /// <see cref="PbsLocalizedTextAttribute"/> with the namespace "PokemonTypes"
    /// and a formatted key specific to the type. Defaults to "Unnamed" if not explicitly set.
    /// </summary>
    [PbsLocalizedText("PokemonTypes", "{0}_Name")]
    public FText Name { get; init; } = "Unnamed";

    /// <summary>
    /// Gets a value indicating whether the type is classified as a special type.
    /// This classification may be used for certain game mechanics or behaviors specific to special types.
    /// </summary>
    public bool IsSpecialType { get; init; }

    /// <summary>
    /// Indicates whether the type is classified as a pseudo type.
    /// This property determines if the type possesses characteristics
    /// that differentiate it from standard types, potentially impacting
    /// specific behaviors within the context of type-related operations.
    /// </summary>
    public bool IsPseudoType { get; init; }

    /// <summary>
    /// Gets a collection of type identifiers representing the weaknesses associated with this type.
    /// Each identifier corresponds to another type that this type is weak against in gameplay mechanics.
    /// </summary>
    public IReadOnlyList<FName> Weaknesses { get; init; } = [];

    /// <summary>
    /// Gets the list of type identifiers that this type resists. A resisted type deals reduced
    /// damage when attacking this type. This property helps in determining type advantage scenarios
    /// for damage calculations and gameplay mechanics.
    /// </summary>
    public IReadOnlyList<FName> Resistances { get; init; } = [];

    /// <summary>
    /// Gets the list of type immunities associated with the Type information. This property defines
    /// which types this particular type is completely immune to during interactions.
    /// </summary>
    public IReadOnlyList<FName> Immunities { get; init; } = [];

    /// <summary>
    /// Gets or sets the collection of gameplay tags associated with the type.
    /// This property is marked with the <see cref="PbsNameAttribute"/> to indicate
    /// its serialization name and with the <see cref="PbsGameplayTag"/> attribute
    /// to denote its namespace and creation behavior for gameplay tag data.
    /// </summary>
    [PbsName("Flags")]
    [PbsGameplayTag("Data.Type", Create = true)]
    public FGameplayTagContainer Tags { get; init; } = new() { GameplayTags = [], ParentTags = [] };
}
