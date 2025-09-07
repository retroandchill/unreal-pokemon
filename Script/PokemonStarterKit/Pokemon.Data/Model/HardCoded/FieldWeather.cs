using GameAccessTools.SourceGenerator.Attributes;
using GameDataAccessTools.Core;
using GameDataAccessTools.Core.Utilities;
using JetBrains.Annotations;
using LanguageExt;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.GameDataAccessTools;

namespace Pokemon.Data.Model.HardCoded;

[UStruct]
[CreateStructView]
public readonly partial struct FFieldWeather : IGameDataEntry
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

    [field: UProperty(PropertyFlags.BlueprintReadOnly)]
    public Option<FName> BattleWeather { get; init; }
}

[UClass]
[GameDataRepository<FFieldWeather>]
[UsedImplicitly]
public partial class UFieldWeatherRepository : UStaticGameDataRepository;

[UStruct]
[DataHandle(typeof(GameData), nameof(GameData.FieldWeathers))]
public readonly partial record struct FFieldWeatherHandle;

public static class FieldWeatherExtensions
{
    private const string LocalizationNamespace = "GameData.FieldWeather";

    public static UGameDataManager AddFieldWeathers(this UGameDataManager manager)
    {
        manager.FieldWeathers.RegisterEntry(
            new FFieldWeather
            {
                ID = FFieldWeather.Rain,
                Name = FText.Localized(LocalizationNamespace, "Rain", "Rain"),
                BattleWeather = new FName("Rain"),
            }
        );

        manager.FieldWeathers.RegisterEntry(
            new FFieldWeather
            {
                ID = FFieldWeather.Storm,
                Name = FText.Localized(LocalizationNamespace, "Storm", "Storm"),
                BattleWeather = new FName("Rain"),
            }
        );

        manager.FieldWeathers.RegisterEntry(
            new FFieldWeather
            {
                ID = FFieldWeather.Snow,
                Name = FText.Localized(LocalizationNamespace, "Snow", "Snow"),
                BattleWeather = new FName("Hail"),
            }
        );

        manager.FieldWeathers.RegisterEntry(
            new FFieldWeather
            {
                ID = FFieldWeather.Blizzard,
                Name = FText.Localized(LocalizationNamespace, "Blizzard", "Blizzard"),
                BattleWeather = new FName("Hail"),
            }
        );

        manager.FieldWeathers.RegisterEntry(
            new FFieldWeather
            {
                ID = FFieldWeather.Sandstorm,
                Name = FText.Localized(LocalizationNamespace, "Sandstorm", "Sandstorm"),
                BattleWeather = new FName("Sandstorm"),
            }
        );

        manager.FieldWeathers.RegisterEntry(
            new FFieldWeather
            {
                ID = FFieldWeather.HeavyRain,
                Name = FText.Localized(LocalizationNamespace, "HeavyRain", "HeavyRain"),
                BattleWeather = new FName("Rain"),
            }
        );

        manager.FieldWeathers.RegisterEntry(
            new FFieldWeather
            {
                ID = FFieldWeather.Sun,
                Name = FText.Localized(LocalizationNamespace, "Sun", "Sun"),
                BattleWeather = new FName("Sun"),
            }
        );

        manager.FieldWeathers.RegisterEntry(
            new FFieldWeather
            {
                ID = FFieldWeather.Fog,
                Name = FText.Localized(LocalizationNamespace, "Fog", "Fog"),
                BattleWeather = new FName("Fog"),
            }
        );

        return manager;
    }
}
