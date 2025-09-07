using GameAccessTools.SourceGenerator.Attributes;
using GameDataAccessTools.Core;
using GameDataAccessTools.Core.Views;
using JetBrains.Annotations;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.CoreUObject;
using UnrealSharp.GameDataAccessTools;

namespace Pokemon.Data.HardCoded;

[UStruct]
[CreateStructView]
public readonly partial struct FGrowthRate : IGameDataEntry
{
    public static int MaxLevel => UObject.GetDefault<UPokemonDataSettings>().MaxLevel;

    [UsedImplicitly]
    [field: UProperty(PropertyFlags.BlueprintReadOnly)]
    public required FName ID { get; init; }

    [UsedImplicitly]
    [field: UProperty(PropertyFlags.BlueprintReadOnly)]
    public int RowIndex { get; init; }

    [UsedImplicitly]
    [field: UProperty(PropertyFlags.BlueprintReadOnly)]
    public required FText Name { get; init; }
}

[UClass]
[GameDataRepository<FGrowthRate>]
[UsedImplicitly]
public partial class UGrowthRateRepository : UStaticGameDataRepository;

public static class GrowthRateExtensions
{
    public static UGameDataManager AddGrowthRates(this UGameDataManager manager)
    {
        manager.GrowthRates.RegisterEntry(
            new FGrowthRate { ID = FGrowthRate.Medium, Name = "Medium" }
        );
        manager.GrowthRates.RegisterEntry(
            new FGrowthRate { ID = FGrowthRate.Fluctuating, Name = "Fluctuating" }
        );
        manager.GrowthRates.RegisterEntry(
            new FGrowthRate { ID = FGrowthRate.Parabolic, Name = "Parabolic" }
        );
        manager.GrowthRates.RegisterEntry(new FGrowthRate { ID = FGrowthRate.Fast, Name = "Fast" });
        manager.GrowthRates.RegisterEntry(new FGrowthRate { ID = FGrowthRate.Slow, Name = "Slow" });

        return manager;
    }
}
