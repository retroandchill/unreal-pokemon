using GameDataAccessTools.Core.Utilities;
using Pokemon.UI.Screens;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.Core;
using UnrealSharp.DeveloperSettings;
using UnrealSharp.UnrealSharpCore;

namespace Pokemon.UI;

/// <summary>
/// Represents the default configuration settings for the Pokémon UI elements within the game.
/// This class provides configuration options for key UI screens, such as the Pokémon selection screen
/// and the bag screen, allowing their class references to be defined and managed.
/// </summary>
[UClass(ClassFlags.DefaultConfig, ConfigCategory = "Game")]
public class UPokemonUISettings : UCSDeveloperSettings
{
    /// <summary>
    /// Represents the class reference for the Pokémon selection screen widget.
    /// This property is used to configure and define the class to be used for the Pokémon selection screen,
    /// allowing the gameplay UI to dynamically instantiate and display the screen when required.
    /// </summary>
    [UProperty(
        PropertyFlags.EditDefaultsOnly | PropertyFlags.BlueprintReadOnly | PropertyFlags.Config,
        Category = "Classes"
    )]
    public TSoftClassPtr<UPokemonSelectScreen> PokemonSelectScreenClass { get; }

    /// <summary>
    /// Represents the class reference for the Bag screen widget.
    /// This property is used to define and configure the class to be instantiated
    /// and displayed for managing the in-game bag UI during gameplay.
    /// </summary>
    [UProperty(
        PropertyFlags.EditDefaultsOnly | PropertyFlags.BlueprintReadOnly | PropertyFlags.Config,
        Category = "Classes"
    )]
    public TSoftClassPtr<UBagScreen> BagScreenClass { get; }

    /// <inheritdoc />
    protected override FText GetSectionText_Implementation()
    {
        return FText.Localized("PokemonUI", "PokemonUI", "Pokémon UI");
    }
}
