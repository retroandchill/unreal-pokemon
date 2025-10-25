using Pokemon.Data.Model.HardCoded;
using Pokemon.Data.Model.PBS;
using UnrealSharp;
using UnrealSharp.Attributes;

namespace Pokemon.Core.Model.Pokemon;

/// <summary>
/// Represents the identity information of a Pokémon.
/// </summary>
[UStruct]
public readonly partial record struct FIdentityComponentInfo
{
    /// <summary>
    /// Gets the species identity of the Pokémon.
    /// </summary>
    /// <remarks>
    /// This property is used to identify the specific species of a Pokémon based on its data handle.
    /// It is part of the Pokémon's core identity information within the game system.
    /// </remarks>
    [UProperty(PropertyFlags.SaveGame)]
    public required FSpeciesHandle Species { get; init; }

    /// <summary>
    /// Gets the nickname of the Pokémon.
    /// </summary>
    /// <remarks>
    /// This property defines the personalized name given to a Pokémon by its Trainer.
    /// It provides a way to distinguish a specific Pokémon beyond its species identity.
    /// </remarks>
    [UProperty(PropertyFlags.SaveGame)]
    public required FText Nickname { get; init; }

    /// <summary>
    /// Gets the personality value of the Pokémon.
    /// </summary>
    /// <remarks>
    /// This property represents an essential identifier used to distinguish individual Pokémon.
    /// It is typically utilized in various mechanics, such as nature determination, random event generation, and feature uniqueness.
    /// </remarks>
    [UProperty(PropertyFlags.SaveGame)]
    public required uint PersonalityValue { get; init; }

    /// <summary>
    /// Gets the gender of the Pokémon.
    /// </summary>
    /// <remarks>
    /// This property indicates the gender identity of the Pokémon, which can be male, female, or genderless.
    /// It plays a role in certain mechanics such as breeding and interactions involving gender-dependent behavior.
    /// </remarks>
    [UProperty(PropertyFlags.SaveGame)]
    public required EPokemonGender Gender { get; init; }

    /// <summary>
    /// Indicates whether the Pokémon is shiny or not.
    /// </summary>
    /// <remarks>
    /// A shiny Pokémon is considered a rare variant with a different color palette than its usual design.
    /// This property is a key attribute that determines if a specific Pokémon instance exhibits this special appearance.
    /// </remarks>
    [UProperty(PropertyFlags.SaveGame)]
    public required bool IsShiny { get; init; }

    /// <summary>
    /// Indicates whether the Pokémon is classified as "Super Shiny."
    /// </summary>
    /// <remarks>
    /// A "Super Shiny" Pokémon is a rarer variant of a shiny Pokémon, signifying an even more special status.
    /// This property provides additional identification for exceptionally unique Pokémon belonging to this category.
    /// </remarks>
    [UProperty(PropertyFlags.SaveGame)]
    public required bool IsSuperShiny { get; init; }
}
