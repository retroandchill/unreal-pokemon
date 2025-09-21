using Pokemon.Core.Entities;
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
}