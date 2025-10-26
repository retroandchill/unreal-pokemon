using GameDataAccessTools.Core.Utilities;
using Pokemon.Core.Entities;
using Pokemon.Data;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.Attributes.MetaTags;
using UnrealSharp.Core;
using UnrealSharp.GameplayTags;
using UnrealSharp.UnrealSharpCore;

namespace Pokemon.Core;

/// <summary>
/// Represents the configuration settings for a pocket, including its associated tag, name, and optional maximum size.
/// This struct is primarily used in the Pokemon.Core namespace for managing individual pocket configurations.
/// </summary>
/// <param name="Tag">The gameplay tag associated with the pocket.</param>
/// <param name="Name">The name of the pocket.</param>
/// <param name="MaxSize">The maximum number of items that can be stored in the pocket.</param>
[UStruct]
public readonly partial record struct FPocketSetting(
    [property: UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere)]
    [property: Categories(IdentifierConstants.PocketTag)]
        FGameplayTag Tag,
    [property: UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere)] FText Name,
    [property: UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere)] TOptional<int> MaxSize
);

/// <summary>
/// Represents the core configuration settings for the Pokémon system.
/// This class provides configurable properties and default classes for key gameplay elements such as trainers, Pokémon, and inventory management.
/// It is intended to centralize the settings used throughout the Pokémon game framework.
/// </summary>
[UClass(ClassFlags.DefaultConfig, DisplayName = "Pokémon Core", ConfigCategory = "Game")]
public partial class UPokemonCoreSettings : UCSDeveloperSettings
{
    /// <summary>
    /// Represents the maximum number of Pokémon allowed in a trainer's party.
    /// This value defines the upper limit of Pokémon that can be actively carried by a trainer at any given time.
    /// </summary>
    [UProperty(
        PropertyFlags.EditDefaultsOnly | PropertyFlags.BlueprintReadOnly | PropertyFlags.Config,
        Category = "Trainers"
    )]
    public partial int MaxPartySize { get; }

    /// <summary>
    /// Defines the maximum number of moves a Pokémon can learn or have at any given time.
    /// This value determines the limit on the number of active moves in a Pokémon's moveset.
    /// </summary>
    [UProperty(
        PropertyFlags.EditDefaultsOnly | PropertyFlags.BlueprintReadOnly | PropertyFlags.Config,
        Category = "Pokémon"
    )]
    public partial int MaxMoves { get; }

    /// <summary>
    /// Represents the base chance of a Pokémon being shiny, expressed as a specific value.
    /// This value determines the likelihood of encountering or generating shiny Pokémon in the game.
    /// A lower value increases the rarity of shiny Pokémon.
    /// </summary>
    [UProperty(
        PropertyFlags.EditDefaultsOnly | PropertyFlags.BlueprintReadOnly | PropertyFlags.Config,
        Category = "Pokémon"
    )]
    public partial int ShinyPokemonChance { get; }

    /// <summary>
    /// Contains the list of all pocket settings available in the game.
    /// Each pocket represents a categorization of items within the player's inventory system,
    /// defined by attributes such as tag, name, and maximum size.
    /// </summary>
    [UProperty(
        PropertyFlags.EditDefaultsOnly | PropertyFlags.BlueprintReadOnly | PropertyFlags.Config,
        Category = "Bag"
    )]
    public partial TArray<FPocketSetting> Pockets { get; }

    /// <summary>
    /// Represents the maximum quantity of a single item type that can be stored in one inventory slot.
    /// This property defines the upper limit for item stacking within the Pokémon bag system.
    /// </summary>
    [UProperty(
        PropertyFlags.EditDefaultsOnly | PropertyFlags.BlueprintReadOnly | PropertyFlags.Config,
        Category = "Bag"
    )]
    public partial int MaxPerSlot { get; }

    /// <summary>
    /// Represents the default class for Pokémon in the game.
    /// This property defines the blueprint or archetype used for creating Pokémon instances.
    /// </summary>
    [UProperty(
        PropertyFlags.EditDefaultsOnly | PropertyFlags.BlueprintReadOnly | PropertyFlags.Config,
        Category = "DefaultClasses"
    )]
    public partial TSoftClassPtr<UPokemon> PokemonClass { get; }

    /// <summary>
    /// Specifies the default class type for trainers in the Pokémon game framework.
    /// This property defines the blueprint or class used as the base implementation for trainer entities.
    /// It allows for customization or extension of the default trainer behavior within the game.
    /// </summary>
    [UProperty(
        PropertyFlags.EditDefaultsOnly | PropertyFlags.BlueprintReadOnly | PropertyFlags.Config,
        Category = "DefaultClasses"
    )]
    public partial TSoftClassPtr<UTrainer> TrainerClass { get; }

    /// <summary>
    /// Represents the default class type for the Pokémon Bag within the game settings.
    /// This property determines the class that defines the bag carried by trainers,
    /// which organizes and holds items during gameplay and is configured through default settings.
    /// </summary>
    [UProperty(
        PropertyFlags.EditDefaultsOnly | PropertyFlags.BlueprintReadOnly | PropertyFlags.Config,
        Category = "DefaultClasses"
    )]
    public partial TSoftClassPtr<UPokemonBag> BagClass { get; }

    /// <inheritdoc />
    protected override FText GetSectionText_Implementation()
    {
        return FText.Localized("PokemonCore", "PokemonCore", "Pokémon Core");
    }
}
