using GameAccessTools.SourceGenerator.Attributes;
using GameDataAccessTools.Core;
using GameDataAccessTools.Core.Utilities;
using JetBrains.Annotations;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.Core;
using UnrealSharp.GameDataAccessTools;

namespace Pokemon.Data.Model.HardCoded;

/// <summary>
/// Represents a battle weather configuration in the game.
/// The structure provides details about a specific type of weather
/// encountered during battles, such as its unique identifier, name,
/// and associated row index for internal game data mapping.
/// </summary>
[UStruct]
[CreateStructView]
public readonly partial struct FBattleWeather : IGameDataEntry
{
    /// <inheritdoc />
    [UProperty(PropertyFlags.BlueprintReadOnly)]
    public required FName ID { get; init; }

    /// <inheritdoc />
    [UProperty(PropertyFlags.BlueprintReadOnly)]
    public int RowIndex { get; init; }

    /// <summary>
    /// Gets the name of the battle weather configuration.
    /// This property represents the localized display name of a specific type of weather
    /// encountered during battles, allowing for meaningful and user-friendly representations.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly)]
    [DisplayName]
    public required FText Name { get; init; }
}

/// <summary>
/// Represents a repository responsible for managing, accessing, and organizing
/// battle weather data in the game. This class is derived from
/// a static game data repository and provides functionality to handle
/// structured weather details for battles.
/// </summary>
[UClass]
[GameDataRepository<FBattleWeather>]
public partial class UBattleWeatherRepository : UStaticGameDataRepository;

/// <summary>
/// Represents a handle for accessing battle weather data within the game.
/// This structure serves as a reference to specific battle weather configurations,
/// enabling efficient retrieval and manipulation of related game data.
/// </summary>
[UStruct]
[DataHandle(typeof(GameData), nameof(GameData.BattleWeathers))]
public readonly partial record struct FBattleWeatherHandle ([property: UProperty(PropertyFlags.EditAnywhere)] FName ID);

/// <summary>
/// Provides extension methods for managing battle weather configurations
/// within the game data manager. This class allows the registration
/// and initialization of different types of battle weathers used in battles.
/// </summary>
public static class BattleWeatherExtensions
{
    private const string LocalizationNamespace = "GameData.BattleWeather";

    /// <summary>
    /// Registers predefined battle weather entries to the game data manager.
    /// </summary>
    /// <param name="manager">The game data manager instance to which the battle weather entries will be added.</param>
    /// <returns>The updated game data manager instance with the specified battle weather entries added.</returns>
    public static UGameDataManager AddBattleWeathers(this UGameDataManager manager)
    {
        manager.BattleWeathers.RegisterEntry(
            new FBattleWeather { ID = FBattleWeather.Sun, Name = FText.Localized(LocalizationNamespace, "Sun", "Sun") }
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
