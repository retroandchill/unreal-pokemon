using Pokemon.Editor.Serialization.Pbs.Attributes;
using UnrealSharp;
using UnrealSharp.Core;
using UnrealSharp.GameplayTags;

namespace Pokemon.Editor.Serialization.Model.Pbs;

/// <summary>
/// Represents detailed information about a Pokémon ability, including its ID, name, description,
/// and associated tags for categorization or special properties.
/// </summary>
/// <remarks>
/// This record is primarily used as a data container for Pokémon ability definitions during serialization
/// and deserialization of PBS (Pokémon Battle Simulator) files. It includes attributes to assist with
/// localization, gameplay tagging, and unique key identification.
/// </remarks>
public record AbilityInfo
{
    /// <summary>
    /// Gets the unique identifier for the ability.
    /// </summary>
    /// <remarks>
    /// This property serves as the primary key used to identify the ability in data serialization.
    /// It is decorated with the <see cref="PbsKeyAttribute"/> to mark it as the key property for
    /// Pokémon Battle Simulator (PBS) operations.
    /// </remarks>
    [PbsKey]
    public FName ID { get; init; }

    /// <summary>
    /// Gets the index of the ability within the Pokémon Battle Simulator (PBS) data file.
    /// </summary>
    /// <remarks>
    /// This property is used to determine the zero-based position of the ability entry in the PBS dataset.
    /// It is annotated with the <see cref="PbsIndexAttribute"/> to identify it as a key index used
    /// during serialization, deserialization, and other PBS operations.
    /// </remarks>
    [PbsIndex]
    public int RowIndex { get; init; }

    /// <summary>
    /// Gets the localized name of the Pokémon ability.
    /// </summary>
    /// <remarks>
    /// This property represents the display name of the ability and is subject to localization.
    /// It uses the <see cref="PbsLocalizedTextAttribute"/> to define a namespace and a key format
    /// for retrieving the localized text. The default value is "Unnamed" to ensure that all abilities
    /// have a placeholder name before localization.
    /// </remarks>
    [PbsLocalizedText("PokemonAbilities", "{0}_DisplayName")]
    public FText Name { get; init; } = "Unnamed";

    /// <summary>
    /// Gets the localized description of the ability.
    /// </summary>
    /// <remarks>
    /// This property provides a human-readable text that describes the effect or nature of the ability.
    /// The description is localized using the <see cref="PbsLocalizedTextAttribute"/> with the namespace
    /// "PokemonAbilities" and a key format dependent on the ability's identifier.
    /// It plays a crucial role in the presentation layer of the application by conveying detailed
    /// information about the ability to the user.
    /// </remarks>
    [PbsLocalizedText("PokemonAbilities", "{0}_Description")]
    public FText Description { get; init; } = "???";

    /// <summary>
    /// Gets the gameplay tag container associated with the ability.
    /// </summary>
    /// <remarks>
    /// This property represents a collection of gameplay tags used to classify or assign
    /// specific metadata to the ability. The tags are primarily utilized for gameplay logic,
    /// categorization, or special effects associated with the ability within the Pokémon
    /// Battle Simulator (PBS).
    /// It is decorated with the <see cref="PbsNameAttribute"/> and <see cref="PbsGameplayTag"/>
    /// attributes for naming and dynamic tag creation during serialization.
    /// </remarks>
    [PbsName("Flags")]
    [PbsGameplayTag("Data.Ability", Create = true)]
    public FGameplayTagContainer Tags { get; init; } = new() { GameplayTags = [], ParentTags = [] };
}
