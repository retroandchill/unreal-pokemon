using GameAccessTools.SourceGenerator.Attributes;
using GameDataAccessTools.Core;
using GameDataAccessTools.Core.Utilities;
using JetBrains.Annotations;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.CoreUObject;
using UnrealSharp.GameDataAccessTools;

namespace Pokemon.Data.Model.HardCoded;

/// <summary>
/// Represents a growth rate in the game, defining properties for identifying and describing the rate.
/// </summary>
[UStruct]
[CreateStructView]
public readonly partial struct FGrowthRate : IGameDataEntry
{
    /// <summary>
    /// Gets the maximum allowed level for a Pokémon in the system.
    /// </summary>
    /// <remarks>
    /// The maximum level is defined in the Pokémon data settings and serves as
    /// the upper limit for level progression across various game components,
    /// including experience calculations and stat adjustments. This value is
    /// globally referenced to ensure consistency.
    /// </remarks>
    public static int MaxLevel => UObject.GetDefault<UPokemonDataSettings>().MaxLevel;

    /// <inheritdoc />
    [UsedImplicitly]
    [field: UProperty(PropertyFlags.BlueprintReadOnly)]
    public required FName ID { get; init; }

    /// <inheritdoc />
    [UsedImplicitly]
    [field: UProperty(PropertyFlags.BlueprintReadOnly)]
    public int RowIndex { get; init; }

    /// <summary>
    /// Gets the localized name associated with the growth rate.
    /// </summary>
    /// <remarks>
    /// The name is a user-facing, localized string intended for display purposes
    /// in the game UI or other components where the growth rate needs to be identified
    /// descriptively. Localization ensures that the name is properly translated
    /// and presented in various supported languages.
    /// </remarks>
    [UsedImplicitly]
    [field: UProperty(PropertyFlags.BlueprintReadOnly)]
    [DisplayName]
    public required FText Name { get; init; }
}

/// <summary>
/// Provides data repository functionality for accessing and managing FGrowthRate game data entries.
/// </summary>
[UClass]
[GameDataRepository<FGrowthRate>]
[UsedImplicitly]
public partial class UGrowthRateRepository : UStaticGameDataRepository;

/// <summary>
/// Represents a handle that provides a reference to a specific growth rate within the game data system.
/// This type is primarily used to reference entries in the GrowthRates repository.
/// </summary>
[UStruct]
[DataHandle(typeof(GameData), nameof(GameData.GrowthRates))]
public readonly partial record struct FGrowthRateHandle;

/// <summary>
/// Provides extension methods for managing and interacting with growth rate entries within the game data manager.
/// </summary>
public static class GrowthRateExtensions
{
    private const string LocalizationNamespace = "GameData.GrowthRate";

    /// Adds predefined growth rate entries to the game data manager.
    /// <param name="manager">
    /// The instance of <see cref="UGameDataManager"/> to which growth rate entries will be added.
    /// </param>
    /// <returns>
    /// Returns the updated <see cref="UGameDataManager"/> instance after adding growth rate entries.
    /// </returns>
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
            new FGrowthRate { ID = FGrowthRate.Fast, Name = FText.Localized(LocalizationNamespace, "Fast", "Fast") }
        );

        manager.GrowthRates.RegisterEntry(
            new FGrowthRate { ID = FGrowthRate.Slow, Name = FText.Localized(LocalizationNamespace, "Slow", "Slow") }
        );

        return manager;
    }
}
