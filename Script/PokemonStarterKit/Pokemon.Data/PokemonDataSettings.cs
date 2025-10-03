using UnrealSharp.Attributes;
using UnrealSharp.Attributes.MetaTags;
using UnrealSharp.DeveloperSettings;

namespace Pokemon.Data;

/// <summary>
/// Represents the Pokémon data settings used within the game.
/// This class extends <c>UDeveloperSettings</c> and provides configuration options
/// such as maximum level and move categorization rules.
/// </summary>
[UClass(ClassFlags.DefaultConfig, DisplayName = "Pokémon Data", ConfigCategory = "Game")]
public class UPokemonDataSettings : UDeveloperSettings
{
    /// <summary>
    /// Represents the maximum allowable level for a Pokémon in the game.
    /// Configurable within the Pokémon Data settings, this property determines
    /// the highest level a Pokémon can reach.
    /// </summary>
    [UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditDefaultsOnly | PropertyFlags.Config,
        Category = "Gameplay"
    )]
    [ClampMin("1")]
    public int MaxLevel { get; init; } = 100;

    /// <summary>
    /// Determines whether each Pokémon move is categorized individually based on specific settings.
    /// When enabled, the move category (e.g., Physical or Special) is determined per move rather than using broader rules.
    /// This property provides additional granularity in gameplay mechanics and affects how moves are inherently classified.
    /// </summary>
    [UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditDefaultsOnly | PropertyFlags.Config,
        Category = "Gameplay"
    )]
    public bool MoveCategoryPerMove { get; init; } = false;
}
