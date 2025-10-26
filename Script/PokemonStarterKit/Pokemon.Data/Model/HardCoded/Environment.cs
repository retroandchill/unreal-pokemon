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
/// Represents an environment in the game, containing essential data such as
/// its unique identifier, row index, name, and associated battle base.
/// This struct is primarily used for defining various environmental contexts
/// within the game world.
/// </summary>
[UStruct]
[CreateStructView]
public readonly partial struct FEnvironment : IGameDataEntry
{
    /// <inheritdoc />
    [UProperty(PropertyFlags.BlueprintReadOnly)]
    public required FName ID { get; init; }

    /// <inheritdoc />
    [UProperty(PropertyFlags.BlueprintReadOnly)]
    public int RowIndex { get; init; }

    /// <summary>
    /// Gets the name of the environment as a localized text.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly)]
    [DisplayName]
    public required FText Name { get; init; }

    /// <summary>
    /// The battle base associated with the environment.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly)]
    public required FName BattleBase { get; init; }
}

/// <summary>
/// Represents a repository for managing and accessing static game data
/// related to environmental contexts within the Pokemon game.
/// This class serves as a specialized implementation of the static game data repository
/// for the <c>FEnvironment</c> struct, providing access and management mechanisms for predefined data entries.
/// </summary>
[UClass]
[GameDataRepository<FEnvironment>]
public partial class UEnvironmentRepository : UStaticGameDataRepository;

/// <summary>
/// Represents a handle to an environment data entry within the game's dataset.
/// This struct is used in conjunction with the game data system to reference
/// specific environment entries efficiently. It provides a lightweight and
/// immutable identifier for accessing environment-related data.
/// </summary>
[DataHandle(typeof(GameData), nameof(GameData.Environments))]
public readonly partial record struct FEnvironmentHandle;

/// <summary>
/// Defines a set of predefined environmental battle contexts,
/// represented by static fields, such as grass, water, sand, and others.
/// These contexts are primarily utilized to categorize and handle
/// various terrains that influence game mechanics.
/// </summary>
public static class BattleBase
{
    /// <summary>
    /// Represents the environmental terrain type "grass" used in battles.
    /// </summary>
    public static readonly FName Grass = "grass";

    /// <summary>
    /// Represents the identifier for the water environment type in the battle system.
    /// </summary>
    public static readonly FName Water = "water";

    /// <summary>
    /// Represents the predefined constant "puddle" used within the battle environment setup.
    /// </summary>
    public static readonly FName Puddle = "puddle";

    /// <summary>
    /// Represents the name identifier for the "sand" environment.
    /// </summary>
    public static readonly FName Sand = "sand";

    /// <summary>
    /// Represents the ice type name used within the battle environment.
    /// </summary>
    public static readonly FName Ice = "ice";
}

/// <summary>
/// Provides extension methods for managing and adding environment-related data
/// to a game data manager. This class is used to enable seamless integration
/// of environment configurations within the game's data management system.
/// </summary>
public static class EnvironmentExtensions
{
    private const string LocalizationNamespace = "GameData.Environment";

    /// <summary>
    /// Adds predefined environments configurations to the game data manager.
    /// </summary>
    /// <param name="manager">An instance of <c>UGameDataManager</c> to which the environments will be added.</param>
    /// <returns>Returns the updated instance of <c>UGameDataManager</c> after adding the environments.</returns>
    public static UGameDataManager AddEnvironments(this UGameDataManager manager)
    {
        manager.Environments.RegisterEntry(
            new FEnvironment
            {
                ID = FEnvironment.Grass,
                Name = FText.Localized(LocalizationNamespace, "Grass", "Grass"),
                BattleBase = BattleBase.Grass,
            }
        );

        manager.Environments.RegisterEntry(
            new FEnvironment
            {
                ID = FEnvironment.TallGrass,
                Name = FText.Localized(LocalizationNamespace, "TallGrass", "Tall grass"),
                BattleBase = BattleBase.Grass,
            }
        );

        manager.Environments.RegisterEntry(
            new FEnvironment
            {
                ID = FEnvironment.MovingWater,
                Name = FText.Localized(LocalizationNamespace, "MovingWater", "Moving water"),
                BattleBase = BattleBase.Water,
            }
        );

        manager.Environments.RegisterEntry(
            new FEnvironment
            {
                ID = FEnvironment.StillWater,
                Name = FText.Localized(LocalizationNamespace, "StillWater", "Still water"),
                BattleBase = BattleBase.Water,
            }
        );

        manager.Environments.RegisterEntry(
            new FEnvironment
            {
                ID = FEnvironment.Puddle,
                Name = FText.Localized(LocalizationNamespace, "Puddle", "Puddle"),
                BattleBase = BattleBase.Puddle,
            }
        );

        manager.Environments.RegisterEntry(
            new FEnvironment
            {
                ID = FEnvironment.Underwater,
                Name = FText.Localized(LocalizationNamespace, "Underwater", "Underwater"),
                BattleBase = FName.None,
            }
        );

        manager.Environments.RegisterEntry(
            new FEnvironment
            {
                ID = FEnvironment.Cave,
                Name = FText.Localized(LocalizationNamespace, "Cave", "Cave"),
                BattleBase = FName.None,
            }
        );

        manager.Environments.RegisterEntry(
            new FEnvironment
            {
                ID = FEnvironment.Rock,
                Name = FText.Localized(LocalizationNamespace, "Rock", "Rock"),
                BattleBase = FName.None,
            }
        );

        manager.Environments.RegisterEntry(
            new FEnvironment
            {
                ID = FEnvironment.Sand,
                Name = FText.Localized(LocalizationNamespace, "Sand", "Sand"),
                BattleBase = BattleBase.Sand,
            }
        );

        manager.Environments.RegisterEntry(
            new FEnvironment
            {
                ID = FEnvironment.Forest,
                Name = FText.Localized(LocalizationNamespace, "Forest", "Forest"),
                BattleBase = FName.None,
            }
        );

        manager.Environments.RegisterEntry(
            new FEnvironment
            {
                ID = FEnvironment.ForestGrass,
                Name = FText.Localized(LocalizationNamespace, "ForestGrass", "Forest grass"),
                BattleBase = BattleBase.Grass,
            }
        );

        manager.Environments.RegisterEntry(
            new FEnvironment
            {
                ID = FEnvironment.Snow,
                Name = FText.Localized(LocalizationNamespace, "Snow", "Snow"),
                BattleBase = FName.None,
            }
        );

        manager.Environments.RegisterEntry(
            new FEnvironment
            {
                ID = FEnvironment.Ice,
                Name = FText.Localized(LocalizationNamespace, "Ice", "Ice"),
                BattleBase = BattleBase.Ice,
            }
        );

        manager.Environments.RegisterEntry(
            new FEnvironment
            {
                ID = FEnvironment.Volcano,
                Name = FText.Localized(LocalizationNamespace, "Volcano", "Volcano"),
                BattleBase = FName.None,
            }
        );

        manager.Environments.RegisterEntry(
            new FEnvironment
            {
                ID = FEnvironment.Graveyard,
                Name = FText.Localized(LocalizationNamespace, "Graveyard", "Graveyard"),
                BattleBase = FName.None,
            }
        );

        manager.Environments.RegisterEntry(
            new FEnvironment
            {
                ID = FEnvironment.Sky,
                Name = FText.Localized(LocalizationNamespace, "Sky", "Sky"),
                BattleBase = FName.None,
            }
        );

        manager.Environments.RegisterEntry(
            new FEnvironment
            {
                ID = FEnvironment.Space,
                Name = FText.Localized(LocalizationNamespace, "Space", "Space"),
                BattleBase = FName.None,
            }
        );

        manager.Environments.RegisterEntry(
            new FEnvironment
            {
                ID = FEnvironment.UltraSpace,
                Name = FText.Localized(LocalizationNamespace, "UltraSpace", "Ultra Space"),
                BattleBase = FName.None,
            }
        );

        return manager;
    }
}
