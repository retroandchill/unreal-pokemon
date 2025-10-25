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
/// Represents a nature-based modification applied to a Pokémon's primary battle statistics.
/// This structure defines the specific stat being changed and the percentage by which that stat is modified.
/// </summary>
/// <param name="StatHandle">The handle of the stat being modified.</param>
/// <param name="PercentChange">The percentage by which the stat is modified.</param>
[UStruct]
public readonly partial record struct FNatureStatChange(
    [property: UProperty(PropertyFlags.BlueprintReadOnly)] FMainBattleStatHandle StatHandle,
    [property: UProperty(PropertyFlags.BlueprintReadOnly)] int PercentChange
);

/// <summary>
/// Represents a nature in the Pokémon game data, including its unique identifier, name, and any associated stat modifications.
/// This structure provides the detailed information and properties for a specific nature, such as its effects on a Pokémon's stats.
/// </summary>
[UStruct]
[CreateStructView]
public readonly partial struct FNature() : IGameDataEntry
{
    /// <inheritdoc />
    [UsedImplicitly]
    [UProperty(PropertyFlags.BlueprintReadOnly)]
    public required FName ID { get; init; }

    /// <inheritdoc />
    [UsedImplicitly]
    [UProperty(PropertyFlags.BlueprintReadOnly)]
    public int RowIndex { get; init; }

    /// <summary>
    /// Gets the name of the nature. This property is read-only and intended to provide a localized text representation.
    /// </summary>
    [UsedImplicitly]
    [UProperty(PropertyFlags.BlueprintReadOnly)]
    [DisplayName]
    public required FText Name { get; init; }

    /// <summary>
    /// Gets a read-only list of stat changes associated with this nature.
    /// Each item in the list defines a stat and a corresponding percentage change.
    /// Typically used to describe how a specific nature affects the battle stats of a Pokemon.
    /// </summary>
    [UsedImplicitly]
    [UProperty(PropertyFlags.BlueprintReadOnly)]
    public IReadOnlyList<FNatureStatChange> StatChanges { get; init; } = [];
}

/// <summary>
/// A repository for managing and accessing nature-based game data.
/// This repository provides functionality to query and retrieve hard-coded nature data
/// utilized within the Pokémon application.
/// </summary>
[UClass]
[GameDataRepository<FNature>]
[UsedImplicitly]
public partial class UNatureRepository : UStaticGameDataRepository;

/// <summary>
/// Represents a handle to a specific Pokémon nature configuration.
/// This handle is utilized to identify and retrieve the nature definition from the game data repository.
/// A Pokémon's nature influences its stat modifications during battles, typically boosting one stat while decreasing another.
/// </summary>
[UStruct]
[DataHandle(typeof(GameData), nameof(GameData.Natures))]
public readonly partial record struct FNatureHandle;

/// <summary>
/// Provides extension methods for adding nature-related data and behaviors to the game's data manager.
/// This static class includes functionality for integrating predefined natures into the game system.
/// </summary>
public static class NatureExtensions
{
    private const string LocalizationNamespace = "GameData.Nature";

    /// <summary>
    /// Adds nature data to the specified game data manager instance.
    /// </summary>
    /// <param name="manager">The <see cref="UGameDataManager"/> instance to which the nature data will be added.</param>
    /// <returns>The same <see cref="UGameDataManager"/> instance with the nature data added.</returns>
    public static UGameDataManager AddNatures(this UGameDataManager manager)
    {
        manager.Natures.RegisterEntry(
            new FNature { ID = FNature.HARDY, Name = FText.Localized(LocalizationNamespace, "HARDY", "Hardy") }
        );

        manager.Natures.RegisterEntry(
            new FNature
            {
                ID = FNature.LONELY,
                Name = FText.Localized(LocalizationNamespace, "LONELY", "Lonely"),
                StatChanges = [new FNatureStatChange(FStat.ATTACK, 10), new FNatureStatChange(FStat.DEFENSE, -10)],
            }
        );

        manager.Natures.RegisterEntry(
            new FNature
            {
                ID = FNature.BRAVE,
                Name = FText.Localized(LocalizationNamespace, "BRAVE", "Brave"),
                StatChanges = [new FNatureStatChange(FStat.ATTACK, 10), new FNatureStatChange(FStat.SPEED, -10)],
            }
        );

        manager.Natures.RegisterEntry(
            new FNature
            {
                ID = FNature.ADAMANT,
                Name = FText.Localized(LocalizationNamespace, "ADAMANT", "Adamant"),
                StatChanges =
                [
                    new FNatureStatChange(FStat.ATTACK, 10),
                    new FNatureStatChange(FStat.SPECIAL_ATTACK, -10),
                ],
            }
        );

        manager.Natures.RegisterEntry(
            new FNature
            {
                ID = FNature.NAUGHTY,
                Name = FText.Localized(LocalizationNamespace, "NAUGHTY", "Naughty"),
                StatChanges =
                [
                    new FNatureStatChange(FStat.ATTACK, 10),
                    new FNatureStatChange(FStat.SPECIAL_DEFENSE, -10),
                ],
            }
        );

        manager.Natures.RegisterEntry(
            new FNature
            {
                ID = FNature.BOLD,
                Name = FText.Localized(LocalizationNamespace, "BOLD", "Bold"),
                StatChanges = [new FNatureStatChange(FStat.DEFENSE, 10), new FNatureStatChange(FStat.ATTACK, -10)],
            }
        );

        manager.Natures.RegisterEntry(
            new FNature { ID = FNature.DOCILE, Name = FText.Localized(LocalizationNamespace, "DOCILE", "Docile") }
        );

        manager.Natures.RegisterEntry(
            new FNature
            {
                ID = FNature.RELAXED,
                Name = FText.Localized(LocalizationNamespace, "RELAXED", "Relaxed"),
                StatChanges = [new FNatureStatChange(FStat.DEFENSE, 10), new FNatureStatChange(FStat.SPEED, -10)],
            }
        );

        manager.Natures.RegisterEntry(
            new FNature
            {
                ID = FNature.IMPISH,
                Name = FText.Localized(LocalizationNamespace, "IMPISH", "Impish"),
                StatChanges =
                [
                    new FNatureStatChange(FStat.DEFENSE, 10),
                    new FNatureStatChange(FStat.SPECIAL_ATTACK, -10),
                ],
            }
        );

        manager.Natures.RegisterEntry(
            new FNature
            {
                ID = FNature.LAX,
                Name = FText.Localized(LocalizationNamespace, "LAX", "Lax"),
                StatChanges =
                [
                    new FNatureStatChange(FStat.DEFENSE, 10),
                    new FNatureStatChange(FStat.SPECIAL_DEFENSE, -10),
                ],
            }
        );

        manager.Natures.RegisterEntry(
            new FNature
            {
                ID = FNature.TIMID,
                Name = FText.Localized(LocalizationNamespace, "TIMID", "Timid"),
                StatChanges = [new FNatureStatChange(FStat.SPEED, 10), new FNatureStatChange(FStat.ATTACK, -10)],
            }
        );

        manager.Natures.RegisterEntry(
            new FNature
            {
                ID = FNature.HASTY,
                Name = FText.Localized(LocalizationNamespace, "HASTY", "Hasty"),
                StatChanges = [new FNatureStatChange(FStat.SPEED, 10), new FNatureStatChange(FStat.DEFENSE, -10)],
            }
        );

        manager.Natures.RegisterEntry(
            new FNature { ID = FNature.SERIOUS, Name = FText.Localized(LocalizationNamespace, "SERIOUS", "Serious") }
        );

        manager.Natures.RegisterEntry(
            new FNature
            {
                ID = FNature.JOLLY,
                Name = FText.Localized(LocalizationNamespace, "JOLLY", "Jolly"),
                StatChanges =
                [
                    new FNatureStatChange(FStat.SPEED, 10),
                    new FNatureStatChange(FStat.SPECIAL_ATTACK, -10),
                ],
            }
        );

        manager.Natures.RegisterEntry(
            new FNature
            {
                ID = FNature.NAIVE,
                Name = FText.Localized(LocalizationNamespace, "NAIVE", "Naive"),
                StatChanges =
                [
                    new FNatureStatChange(FStat.SPEED, 10),
                    new FNatureStatChange(FStat.SPECIAL_DEFENSE, -10),
                ],
            }
        );

        manager.Natures.RegisterEntry(
            new FNature
            {
                ID = FNature.MODEST,
                Name = FText.Localized(LocalizationNamespace, "MODEST", "Modest"),
                StatChanges =
                [
                    new FNatureStatChange(FStat.SPECIAL_ATTACK, 10),
                    new FNatureStatChange(FStat.ATTACK, -10),
                ],
            }
        );

        manager.Natures.RegisterEntry(
            new FNature
            {
                ID = FNature.MILD,
                Name = FText.Localized(LocalizationNamespace, "MILD", "Mild"),
                StatChanges =
                [
                    new FNatureStatChange(FStat.SPECIAL_ATTACK, 10),
                    new FNatureStatChange(FStat.DEFENSE, -10),
                ],
            }
        );

        manager.Natures.RegisterEntry(
            new FNature
            {
                ID = FNature.QUIET,
                Name = FText.Localized(LocalizationNamespace, "QUIET", "Quiet"),
                StatChanges =
                [
                    new FNatureStatChange(FStat.SPECIAL_ATTACK, 10),
                    new FNatureStatChange(FStat.SPEED, -10),
                ],
            }
        );

        manager.Natures.RegisterEntry(
            new FNature { ID = FNature.BASHFUL, Name = FText.Localized(LocalizationNamespace, "BASHFUL", "Bashful") }
        );

        manager.Natures.RegisterEntry(
            new FNature
            {
                ID = FNature.RASH,
                Name = FText.Localized(LocalizationNamespace, "RASH", "Rash"),
                StatChanges =
                [
                    new FNatureStatChange(FStat.SPECIAL_ATTACK, 10),
                    new FNatureStatChange(FStat.SPECIAL_DEFENSE, -10),
                ],
            }
        );

        manager.Natures.RegisterEntry(
            new FNature
            {
                ID = FNature.CALM,
                Name = FText.Localized(LocalizationNamespace, "CALM", "Calm"),
                StatChanges =
                [
                    new FNatureStatChange(FStat.SPECIAL_DEFENSE, 10),
                    new FNatureStatChange(FStat.ATTACK, -10),
                ],
            }
        );

        manager.Natures.RegisterEntry(
            new FNature
            {
                ID = FNature.GENTLE,
                Name = FText.Localized(LocalizationNamespace, "GENTLE", "Gentle"),
                StatChanges =
                [
                    new FNatureStatChange(FStat.SPECIAL_DEFENSE, 10),
                    new FNatureStatChange(FStat.DEFENSE, -10),
                ],
            }
        );

        manager.Natures.RegisterEntry(
            new FNature
            {
                ID = FNature.SASSY,
                Name = FText.Localized(LocalizationNamespace, "SASSY", "Sassy"),
                StatChanges =
                [
                    new FNatureStatChange(FStat.SPECIAL_DEFENSE, 10),
                    new FNatureStatChange(FStat.SPEED, -10),
                ],
            }
        );

        manager.Natures.RegisterEntry(
            new FNature
            {
                ID = FNature.CAREFUL,
                Name = FText.Localized(LocalizationNamespace, "CAREFUL", "Careful"),
                StatChanges =
                [
                    new FNatureStatChange(FStat.SPECIAL_DEFENSE, 10),
                    new FNatureStatChange(FStat.SPECIAL_ATTACK, -10),
                ],
            }
        );

        manager.Natures.RegisterEntry(
            new FNature { ID = FNature.QUIRKY, Name = FText.Localized(LocalizationNamespace, "QUIRKY", "Quirky") }
        );

        return manager;
    }
}
