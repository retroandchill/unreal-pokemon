using GameAccessTools.SourceGenerator.Attributes;
using GameDataAccessTools.Core;
using GameDataAccessTools.Core.Utilities;
using JetBrains.Annotations;
using LanguageExt;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.GameDataAccessTools;

namespace Pokemon.Data.Model.HardCoded;

/// <summary>
/// Represents the weather conditions specific to a field in the game. This struct is used to
/// define and retrieve weather-related data tied to a particular field or context.
/// </summary>
[UStruct]
[CreateStructView]
public readonly partial struct FFieldWeather : IGameDataEntry
{
    /// <inheritdoc />
    [UsedImplicitly]
    [field: UProperty(PropertyFlags.BlueprintReadOnly)]
    public required FName ID { get; init; }

    /// <inheritdoc />
    [UsedImplicitly]
    [field: UProperty(PropertyFlags.BlueprintReadOnly)]
    public int RowIndex { get; init; }

    /// <summary>
    /// Represents the name of the field weather. This property is required and provides a localized text value.
    /// </summary>
    [UsedImplicitly]
    [field: UProperty(PropertyFlags.BlueprintReadOnly)]
    [DisplayName]
    public required FText Name { get; init; }

    /// <summary>
    /// Represents the specific type of weather condition that affects battles associated with a field.
    /// This property correlates field-level weather settings to their corresponding battle weather states.
    /// It is an optional value that may not be set for every field.
    /// </summary>
    [field: UProperty(PropertyFlags.BlueprintReadOnly)]
    public Option<FName> BattleWeather { get; init; }
}

/// <summary>
/// Provides a repository for managing and accessing weather-related data specific to fields in the game.
/// This class serves as a static data repository for instances of <see cref="FFieldWeather"/>.
/// </summary>
[UClass]
[GameDataRepository<FFieldWeather>]
[UsedImplicitly]
public partial class UFieldWeatherRepository : UStaticGameDataRepository;

/// <summary>
/// A handle representing a link to a specific instance of field weather data within the game's repository.
/// This struct is used to uniquely reference and interact with an entry from the FieldWeathers dataset.
/// </summary>
[UStruct]
[DataHandle(typeof(GameData), nameof(GameData.FieldWeathers))]
public readonly partial record struct FFieldWeatherHandle;

/// <summary>
/// Provides extension methods for managing and registering field weather types within the game data manager.
/// This static class extends the functionality of <see cref="UGameDataManager"/> to include predefined
/// weather conditions relevant to gameplay.
/// </summary>
public static class FieldWeatherExtensions
{
    private const string LocalizationNamespace = "GameData.FieldWeather";

    /// <summary>
    /// Adds predefined FieldWeather entries to the UGameDataManager instance.
    /// </summary>
    /// <param name="manager">The UGameDataManager instance to which the FieldWeather entries will be added.</param>
    /// <returns>The updated UGameDataManager instance with the added FieldWeather entries.</returns>
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
