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
/// Represents a status entity in the game's hardcoded data model.
/// Provides unique identification, indexing, and display-related properties
/// for a status within the game's system.
/// </summary>
[UStruct]
[CreateStructView]
public readonly partial struct FStatus : IGameDataEntry
{
    /// <inheritdoc />
    [UProperty(PropertyFlags.BlueprintReadOnly)]
    public required FName ID { get; init; }

    /// <inheritdoc />
    [UProperty(PropertyFlags.BlueprintReadOnly)]
    public int RowIndex { get; init; }

    /// <summary>
    /// Specifies the display name of the status within the game's data system.
    /// This property is primarily used for user-facing text and localization.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly)]
    [DisplayName]
    public required FText Name { get; init; }
}

/// <summary>
/// Represents a repository for managing and accessing immutable status data
/// in the game's hardcoded data model. It serves as a concrete implementation
/// to handle status entities derived from the <see cref="FStatus"/> struct.
/// </summary>
[GameDataRepository<FStatus>]
public partial class UStatusRepository : UStaticGameDataRepository;

/// <summary>
/// Represents a unique handle for identifying and referencing a specific status entity
/// within the game's data model.
/// </summary>
[DataHandle(typeof(GameData), nameof(GameData.Statuses))]
public readonly partial record struct FStatusHandle;

/// <summary>
/// Provides extension methods for managing and registering status entities within the game's
/// data management system.
/// </summary>
public static class StatusExtensions
{
    private const string LocalizationNamespace = "GameData.Status";

    /// <summary>
    /// Registers predefined statuses, such as Sleep, Poison, Burn, Paralysis, and Frozen, to the game data manager.
    /// </summary>
    /// <param name="manager">The game data manager instance used to register the statuses.</param>
    /// <returns>The updated game data manager instance after the statuses have been added.</returns>
    public static UGameDataManager AddStatuses(this UGameDataManager manager)
    {
        manager.Statuses.RegisterEntry(
            new FStatus { ID = FStatus.SLEEP, Name = FText.Localized(LocalizationNamespace, "Sleep", "Sleep") }
        );

        manager.Statuses.RegisterEntry(
            new FStatus { ID = FStatus.POISON, Name = FText.Localized(LocalizationNamespace, "Poison", "Poison") }
        );

        manager.Statuses.RegisterEntry(
            new FStatus { ID = FStatus.BURN, Name = FText.Localized(LocalizationNamespace, "Burn", "Burn") }
        );

        manager.Statuses.RegisterEntry(
            new FStatus
            {
                ID = FStatus.PARALYSIS,
                Name = FText.Localized(LocalizationNamespace, "Paralysis", "Paralysis"),
            }
        );

        manager.Statuses.RegisterEntry(
            new FStatus { ID = FStatus.FROZEN, Name = FText.Localized(LocalizationNamespace, "Frozen", "Frozen") }
        );

        return manager;
    }
}
