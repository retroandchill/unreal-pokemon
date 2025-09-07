using GameAccessTools.SourceGenerator.Attributes;
using GameDataAccessTools.Core;
using GameDataAccessTools.Core.Utilities;
using JetBrains.Annotations;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.GameDataAccessTools;

namespace Pokemon.Data.Model.HardCoded;

[UStruct]
[CreateStructView]
public readonly partial struct FBattleWeather : IGameDataEntry
{
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
[GameDataRepository<FBattleWeather>]
[UsedImplicitly]
public partial class UBattleWeatherRepository : UStaticGameDataRepository;

[UStruct]
[DataHandle(typeof(GameData), nameof(GameData.BattleWeathers))]
public readonly partial record struct FBattleWeatherHandle;

public static class BattleWeatherExtensions
{
    private const string LocalizationNamespace = "GameData.BattleWeather";

    public static UGameDataManager AddBattleWeathers(this UGameDataManager manager)
    {
        manager.BattleWeathers.RegisterEntry(
            new FBattleWeather
            {
                ID = FBattleWeather.Sun,
                Name = FText.Localized(LocalizationNamespace, "Sun", "Sun"),
            }
        );

        manager.BattleWeathers.RegisterEntry(
            new FBattleWeather
            {
                ID = FBattleWeather.Rain,
                Name = FText.Localized(LocalizationNamespace, "Rain", "Rain"),
            }
        );

        manager.BattleWeathers.RegisterEntry(
            new FBattleWeather
            {
                ID = FBattleWeather.Sandstorm,
                Name = FText.Localized(LocalizationNamespace, "Sandstorm", "Sandstorm"),
            }
        );

        manager.BattleWeathers.RegisterEntry(
            new FBattleWeather
            {
                ID = FBattleWeather.Hail,
                Name = FText.Localized(LocalizationNamespace, "Hail", "Hail"),
            }
        );

        manager.BattleWeathers.RegisterEntry(
            new FBattleWeather
            {
                ID = FBattleWeather.HarshSun,
                Name = FText.Localized(LocalizationNamespace, "HarshSun", "Harsh Sun"),
            }
        );

        manager.BattleWeathers.RegisterEntry(
            new FBattleWeather
            {
                ID = FBattleWeather.HeavyRain,
                Name = FText.Localized(LocalizationNamespace, "HeavyRain", "Heavy Rain"),
            }
        );

        manager.BattleWeathers.RegisterEntry(
            new FBattleWeather
            {
                ID = FBattleWeather.StrongWinds,
                Name = FText.Localized(LocalizationNamespace, "StrongWinds", "Strong Winds"),
            }
        );

        manager.BattleWeathers.RegisterEntry(
            new FBattleWeather
            {
                ID = FBattleWeather.ShadowSky,
                Name = FText.Localized(LocalizationNamespace, "ShadowSky", "Shadow Sky"),
            }
        );

        return manager;
    }
}
