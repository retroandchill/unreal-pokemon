using Pokemon.Core.Entities;
using Pokemon.Data.Model.PBS;
using UnrealSharp.Attributes;
using UnrealSharp.Attributes.MetaTags;
using UnrealSharp.CoreUObject;
using UnrealSharp.Engine;

namespace Pokemon.Core.Utilities;

[UClass]
public class UPokemonCoreBlueprintLibrary : UBlueprintFunctionLibrary
{
    [UFunction(FunctionFlags.BlueprintPure, Category = "Pokémon|Trainers")]
    [WorldContext(nameof(worldContextObject))]
    public static UTrainer GetPlayer(UObject worldContextObject)
    {
        return worldContextObject.GetGameInstanceSubsystem<UPokemonSubsystem>().Player;
    }

    [UFunction(FunctionFlags.BlueprintCallable, Category = "Pokémon|Party")]
    [WorldContext(nameof(worldContextObject))]
    public static UPokemon AddPokemonToParty(
        UObject worldContextObject,
        FSpeciesHandle species,
        int level = 5
    )
    {
        var player = GetPlayer(worldContextObject);
        var newPokemon = UPokemon.Create(player, species, level);

        // TODO: We need to check if the party is full and if that's the case automatically box the Pokémon
        player.AddPokemon(newPokemon);
        return newPokemon;
    }
}
