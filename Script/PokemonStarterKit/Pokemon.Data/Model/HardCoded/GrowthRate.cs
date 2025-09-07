using GameAccessTools.SourceGenerator.Attributes;
using GameDataAccessTools.Core;
using GameDataAccessTools.Core.Utilities;
using JetBrains.Annotations;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.CoreUObject;
using UnrealSharp.GameDataAccessTools;

namespace Pokemon.Data.Model.HardCoded;

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
    [DisplayName]
    public required FText Name { get; init; }
}

[UClass]
[GameDataRepository<FGrowthRate>]
[UsedImplicitly]
public partial class UGrowthRateRepository : UStaticGameDataRepository;

[UStruct]
[DataHandle(typeof(GameData), nameof(GameData.GrowthRates))]
public readonly partial record struct FGrowthRateHandle;

public static class GrowthRateExtensions
{
    private const string LocalizationNamespace = "GameData.GrowthRate";

    public static UGameDataManager AddGrowthRates(this UGameDataManager manager)
    {
        manager.GrowthRates.RegisterEntry(
            new FGrowthRate
            {
                ID = FGrowthRate.Medium,
                Name = FText.Localized(LocalizationNamespace, "Medium", "Medium"),
            }
        );

        manager.GrowthRates.RegisterEntry(
            new FGrowthRate
            {
                ID = FGrowthRate.Fluctuating,
                Name = FText.Localized(LocalizationNamespace, "Fluctuating", "Fluctuating"),
            }
        );

        manager.GrowthRates.RegisterEntry(
            new FGrowthRate
            {
                ID = FGrowthRate.Parabolic,
                Name = FText.Localized(LocalizationNamespace, "Parabolic", "Parabolic"),
            }
        );

        manager.GrowthRates.RegisterEntry(
            new FGrowthRate
            {
                ID = FGrowthRate.Fast,
                Name = FText.Localized(LocalizationNamespace, "Fast", "Fast"),
            }
        );

        manager.GrowthRates.RegisterEntry(
            new FGrowthRate
            {
                ID = FGrowthRate.Slow,
                Name = FText.Localized(LocalizationNamespace, "Slow", "Slow"),
            }
        );

        return manager;
    }
}
