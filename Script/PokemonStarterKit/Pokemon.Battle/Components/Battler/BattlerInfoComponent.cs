using Pokemon.Core.Entities;
using Pokemon.Data.Model.PBS;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.RPGCore;

namespace Pokemon.Battle.Components.Battler;

/// <summary>
/// Represents a component that provides information about a battler in the context of a Pokémon battle system.
/// </summary>
/// <remarks>
/// This component serves as a container for data related to the battler's properties, such as the Pokémon instance, name, species, and types.
/// It is used within the battle system to handle and retrieve combat-related details of a specific battler.
/// </remarks>
[UClass]
public class UBattlerInfoComponent : URPGComponent
{
    /// <summary>
    /// Represents the core entity for a Pokémon in the system.
    /// </summary>
    /// <remarks>
    /// The Pokemon class is an abstract base class that encapsulates the essential properties and behavior of a Pokémon within the game's architecture.
    /// It serves as the foundational type for all Pokémon-related entities, enabling the integration of components such as identity, stats, moves, and item holding.
    /// This class is designed to be extended or instantiated in game logic to represent individual Pokémon entities.
    /// </remarks>
    [UProperty(PropertyFlags.BlueprintReadWrite, Category = "Battler|Info")]
    public UPokemon Pokemon { get; private set; }

    /// <summary>
    /// Represents the name of the battler as a localized text property.
    /// </summary>
    /// <remarks>
    /// This property holds the display name of the battler associated with the component.
    /// It is used within the Pokémon battle system to reference or display the battler's name in various contexts, such as UI elements or combat logs.
    /// </remarks>
    [UProperty(PropertyFlags.BlueprintReadWrite, Category = "Battler|Info")]
    public FText Name { get; set; }

    /// <summary>
    /// Represents the species information associated with a battler in the Pokémon battle system.
    /// </summary>
    /// <remarks>
    /// The Species property identifies the specific species of the Pokémon associated with the battler.
    /// It serves as a handle to access species-related metadata, which may include attributes
    /// such as base stats, abilities, and evolutions.
    /// This property's data is integral for enabling species-specific logic in battle mechanics and interactions.
    /// </remarks>
    [UProperty(PropertyFlags.BlueprintReadWrite, Category = "Battler|Info")]
    public FSpeciesHandle Species { get; set; }

    /// <summary>
    /// Represents the types associated with a Pokémon in the battle system.
    /// </summary>
    /// <remarks>
    /// This property contains a collection of type handles that define the elemental types of a battler.
    /// The types are used in battle mechanics to determine advantages, disadvantages, and other interactions based on the Pokémon's type combinations.
    /// </remarks>
    [UProperty(PropertyFlags.BlueprintReadWrite, Category = "Battler|Info")]
    public TArray<FTypeHandle> Types { get; }
}
