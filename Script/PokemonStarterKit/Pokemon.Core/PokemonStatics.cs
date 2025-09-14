using Pokemon.Core.Entities;
using Pokemon.Core.Services.Exp;
using Pokemon.Data.Model.HardCoded;
using UnrealSharp.Attributes;
using UnrealSharp.Core;
using UnrealSharp.CoreUObject;
using UnrealSharp.Engine;

namespace Pokemon.Core;

public static class PokemonStatics
{
    public static int MaxPartySize => UObject.GetDefault<UPokemonCoreSettings>().MaxPartySize;

    internal static UObject WorldContextObject
    {
        get
        {
            var worldContextObject = FCSManagerExporter.CallGetCurrentWorldContext();
            var worldContextHandle = FCSManagerExporter.CallFindManagedObject(worldContextObject);
            return GCHandleUtilities.GetObjectFromHandlePtr<UObject>(worldContextHandle)!;
        }
    }

    public static UTrainer Player =>
        WorldContextObject.GetGameInstanceSubsystem<UPokemonSubsystem>().Player;

    public static IExpGrowthFormula GetExpGrowthFormula(FGrowthRateHandle growthRate)
    {
        return WorldContextObject
            .GetGameInstanceSubsystem<UPokemonSubsystem>()
            .GetExpGrowthFormula(growthRate);
    }
}

[UClass]
public class UPokemonStatic : UBlueprintFunctionLibrary
{
    [UFunction(FunctionFlags.BlueprintPure, Category = "Pokémon")]
    public static UTrainer GetPlayer()
    {
        return PokemonStatics.Player;
    }
}
