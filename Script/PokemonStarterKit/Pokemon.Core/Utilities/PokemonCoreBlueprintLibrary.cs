using Pokemon.Core.Entities;
using Pokemon.Data.Model.PBS;
using UnrealSharp.Attributes;
using UnrealSharp.Attributes.MetaTags;
using UnrealSharp.CoreUObject;
using UnrealSharp.Engine;

namespace Pokemon.Core.Utilities;

/// <summary>
/// UPokemonCoreBlueprintLibrary provides a collection of utility functions related to core Pokémon mechanics.
/// These functions enable interaction with trainers, items, and Pokémon in the game world.
/// </summary>
[UClass]
public class UPokemonCoreBlueprintLibrary : UBlueprintFunctionLibrary
{
    /// <summary>
    /// Retrieves the player trainer instance from the game world context.
    /// </summary>
    /// <param name="worldContextObject">The world context object used to locate the game instance and extract the player trainer information.</param>
    /// <returns>The <see cref="UTrainer"/> instance representing the player trainer.</returns>
    [UFunction(FunctionFlags.BlueprintPure, Category = "Pokémon|Trainers")]
    [WorldContext(nameof(worldContextObject))]
    public static UTrainer GetPlayer(UObject worldContextObject)
    {
        return worldContextObject.GetGameInstanceSubsystem<UPokemonSubsystem>().Player;
    }

    /// <summary>
    /// Retrieves the Pokémon bag instance associated with the game context.
    /// </summary>
    /// <param name="worldContextObject">The world context object used to access the game instance and retrieve the Pokémon bag.</param>
    /// <returns>The <see cref="UPokemonBag"/> instance representing the player's Pokémon bag.</returns>
    [UFunction(FunctionFlags.BlueprintPure, Category = "Pokémon|Items")]
    [WorldContext(nameof(worldContextObject))]
    public static UPokemonBag GetBag(UObject worldContextObject)
    {
        return worldContextObject.GetGameInstanceSubsystem<UPokemonSubsystem>().Bag;
    }

    /// <summary>
    /// Adds a new Pokémon to the player's party based on the specified species and level.
    /// If the party is full, the functionality to automatically box the Pokémon is required but not yet implemented.
    /// </summary>
    /// <param name="worldContextObject">The world context object used to locate the game instance and retrieve the player.</param>
    /// <param name="species">The species of the Pokémon to be added to the party.</param>
    /// <param name="level">The level of the Pokémon to be added. Defaults to 5 if not specified.</param>
    /// <returns>The <see cref="UPokemon"/> instance representing the newly added Pokémon.</returns>
    [UFunction(FunctionFlags.BlueprintCallable, Category = "Pokémon|Party")]
    [WorldContext(nameof(worldContextObject))]
    public static UPokemon AddPokemonToParty(UObject worldContextObject, FSpeciesHandle species, int level = 5)
    {
        var player = GetPlayer(worldContextObject);
        var newPokemon = UPokemon.Create(player, species, level);

        // TODO: We need to check if the party is full and if that's the case automatically box the Pokémon
        player.AddPokemon(newPokemon);
        return newPokemon;
    }
}
