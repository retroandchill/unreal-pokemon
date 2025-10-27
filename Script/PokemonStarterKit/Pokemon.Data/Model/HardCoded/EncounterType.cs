using GameAccessTools.SourceGenerator.Attributes;
using GameDataAccessTools.Core;
using GameDataAccessTools.Core.Utilities;
using JetBrains.Annotations;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.Attributes.MetaTags;
using UnrealSharp.Core;
using UnrealSharp.GameDataAccessTools;
using UnrealSharp.GameplayTags;

namespace Pokemon.Data.Model.HardCoded;

/// <summary>
/// Represents various triggers that can initiate an encounter in the game.
/// </summary>
[UEnum]
public enum EEncounterTrigger : byte
{
    /// <summary>
    /// No trigger
    /// </summary>
    None,

    /// <summary>
    /// Encountered in the tall grass.
    /// </summary>
    Land,

    /// <summary>
    /// Cave encounter.
    /// </summary>
    Cave,

    /// <summary>
    /// Encountered while surfing.
    /// </summary>
    Water,

    /// <summary>
    /// Encountered while fishing.
    /// </summary>
    Fishing,

    /// <summary>
    /// Encountered during a Bug Catching Contest.
    /// </summary>
    Contest,
}

/// <summary>
/// Represents an encounter type definition used within the game data structure.
/// </summary>
/// <remarks>
/// This struct provides a way to categorize and define various types of encounters
/// along with their relevant properties and metadata. It implements IGameDataEntry
/// for integration within the game data system.
/// </remarks>
[UStruct]
[CreateStructView]
public readonly partial struct FEncounterType : IGameDataEntry
{
    /// <inheritdoc />
    [UProperty(PropertyFlags.BlueprintReadOnly)]
    public required FName ID { get; init; }

    /// <inheritdoc />
    [UProperty(PropertyFlags.BlueprintReadOnly)]
    public int RowIndex { get; init; }

    /// <summary>
    /// Gets or sets the name associated with the encounter type.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly)]
    [GameAccessTools.SourceGenerator.Attributes.DisplayName]
    public required FText Name { get; init; }

    /// <summary>
    /// Gets the type of encounter trigger associated with this encounter.
    /// </summary>
    /// <remarks>
    /// Possible values are defined in the <see cref="EEncounterTrigger"/> enumeration, such as Land, Cave, Water, etc.
    /// </remarks>
    [UProperty(PropertyFlags.BlueprintReadOnly)]
    [Categories("Field.Encounters.Triggers")]
    public required EEncounterTrigger Type { get; init; }

    /// <summary>
    /// Represents the probability or likelihood of a trigger occurring in the context of an encounter.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly)]
    public TOptional<int> TriggerChance { get; init; }
}

/// <summary>
/// Represents a repository for managing and accessing encounter type data within the game.
/// </summary>
/// <remarks>
/// This class serves as a specialized implementation of UStaticGameDataRepository, designed to provide
/// an interface for retrieving and handling data related to encounter types defined in the game data structure.
/// It operates on the <see cref="FEncounterType"/> type for categorization and data manipulation purposes.
/// </remarks>
[GameDataRepository<FEncounterType>]
public partial class UEncounterTypeRepository : UStaticGameDataRepository;

/// <summary>
/// Represents a handle to an encounter type, providing an indirect reference to specific encounter data.
/// </summary>
/// <remarks>
/// This struct is used to safely reference and work with encounter type data within the game system.
/// It acts as a lightweight identifier or pointer to an existing FEncounterType entry in the data repository.
/// </remarks>
[DataHandle(typeof(GameData), nameof(GameData.EncounterTypes))]
public readonly partial record struct FEncounterTypeHandle;

/// <summary>
/// Provides extension methods for encounter type-related operations within the game data manager.
/// </summary>
/// <remarks>
/// This static class is designed to enhance the functionality of the UGameDataManager
/// by adding methods that facilitate the handling and integration of encounter types.
/// It helps streamline the management of encounter-related data.
/// </remarks>
public static class EncounterTypeExtensions
{
    private const string LocalizationNamespace = "GameData.EncounterType";

    /// <summary>
    /// Adds encounter types to the specified game data manager.
    /// </summary>
    /// <param name="manager">The instance of the game data manager to which encounter types will be added.</param>
    /// <returns>The updated instance of the game data manager with encounter types added.</returns>
    public static UGameDataManager AddEncounterTypes(this UGameDataManager manager)
    {
        manager.EncounterTypes.RegisterEntry(
            new FEncounterType
            {
                ID = FEncounterType.Land,
                Name = FText.Localized(LocalizationNamespace, "Land", "Land"),
                Type = EEncounterTrigger.Land,
                TriggerChance = 21,
            }
        );

        manager.EncounterTypes.RegisterEntry(
            new FEncounterType
            {
                ID = FEncounterType.LandDay,
                Name = FText.Localized(LocalizationNamespace, "LandDay", "LandDay"),
                Type = EEncounterTrigger.Land,
                TriggerChance = 21,
            }
        );

        manager.EncounterTypes.RegisterEntry(
            new FEncounterType
            {
                ID = FEncounterType.LandNight,
                Name = FText.Localized(LocalizationNamespace, "LandNight", "LandNight"),
                Type = EEncounterTrigger.Land,
                TriggerChance = 21,
            }
        );

        manager.EncounterTypes.RegisterEntry(
            new FEncounterType
            {
                ID = FEncounterType.LandMorning,
                Name = FText.Localized(LocalizationNamespace, "LandMorning", "LandMorning"),
                Type = EEncounterTrigger.Land,
                TriggerChance = 21,
            }
        );

        manager.EncounterTypes.RegisterEntry(
            new FEncounterType
            {
                ID = FEncounterType.LandAfternoon,
                Name = FText.Localized(LocalizationNamespace, "LandAfternoon", "LandAfternoon"),
                Type = EEncounterTrigger.Land,
                TriggerChance = 21,
            }
        );

        manager.EncounterTypes.RegisterEntry(
            new FEncounterType
            {
                ID = FEncounterType.LandEvening,
                Name = FText.Localized(LocalizationNamespace, "LandEvening", "LandEvening"),
                Type = EEncounterTrigger.Land,
                TriggerChance = 21,
            }
        );

        manager.EncounterTypes.RegisterEntry(
            new FEncounterType
            {
                ID = FEncounterType.PokeRadar,
                Name = FText.Localized(LocalizationNamespace, "PokeRadar", "PokeRadar"),
                Type = EEncounterTrigger.Land,
                TriggerChance = 20,
            }
        );

        manager.EncounterTypes.RegisterEntry(
            new FEncounterType
            {
                ID = FEncounterType.Cave,
                Name = FText.Localized(LocalizationNamespace, "Cave", "Cave"),
                Type = EEncounterTrigger.Cave,
                TriggerChance = 5,
            }
        );

        manager.EncounterTypes.RegisterEntry(
            new FEncounterType
            {
                ID = FEncounterType.CaveDay,
                Name = FText.Localized(LocalizationNamespace, "CaveDay", "CaveDay"),
                Type = EEncounterTrigger.Cave,
                TriggerChance = 5,
            }
        );

        manager.EncounterTypes.RegisterEntry(
            new FEncounterType
            {
                ID = FEncounterType.CaveNight,
                Name = FText.Localized(LocalizationNamespace, "CaveNight", "CaveNight"),
                Type = EEncounterTrigger.Cave,
                TriggerChance = 5,
            }
        );

        manager.EncounterTypes.RegisterEntry(
            new FEncounterType
            {
                ID = FEncounterType.CaveMorning,
                Name = FText.Localized(LocalizationNamespace, "CaveMorning", "CaveMorning"),
                Type = EEncounterTrigger.Cave,
                TriggerChance = 5,
            }
        );

        manager.EncounterTypes.RegisterEntry(
            new FEncounterType
            {
                ID = FEncounterType.CaveAfternoon,
                Name = FText.Localized(LocalizationNamespace, "CaveAfternoon", "CaveAfternoon"),
                Type = EEncounterTrigger.Cave,
                TriggerChance = 5,
            }
        );

        manager.EncounterTypes.RegisterEntry(
            new FEncounterType
            {
                ID = FEncounterType.CaveEvening,
                Name = FText.Localized(LocalizationNamespace, "CaveEvening", "CaveEvening"),
                Type = EEncounterTrigger.Cave,
                TriggerChance = 5,
            }
        );

        manager.EncounterTypes.RegisterEntry(
            new FEncounterType
            {
                ID = FEncounterType.Water,
                Name = FText.Localized(LocalizationNamespace, "Water", "Water"),
                Type = EEncounterTrigger.Water,
                TriggerChance = 2,
            }
        );

        manager.EncounterTypes.RegisterEntry(
            new FEncounterType
            {
                ID = FEncounterType.WaterDay,
                Name = FText.Localized(LocalizationNamespace, "WaterDay", "WaterDay"),
                Type = EEncounterTrigger.Water,
                TriggerChance = 2,
            }
        );

        manager.EncounterTypes.RegisterEntry(
            new FEncounterType
            {
                ID = FEncounterType.WaterNight,
                Name = FText.Localized(LocalizationNamespace, "WaterNight", "WaterNight"),
                Type = EEncounterTrigger.Water,
                TriggerChance = 2,
            }
        );

        manager.EncounterTypes.RegisterEntry(
            new FEncounterType
            {
                ID = FEncounterType.WaterMorning,
                Name = FText.Localized(LocalizationNamespace, "WaterMorning", "WaterMorning"),
                Type = EEncounterTrigger.Water,
                TriggerChance = 2,
            }
        );

        manager.EncounterTypes.RegisterEntry(
            new FEncounterType
            {
                ID = FEncounterType.WaterAfternoon,
                Name = FText.Localized(LocalizationNamespace, "WaterAfternoon", "WaterAfternoon"),
                Type = EEncounterTrigger.Water,
                TriggerChance = 2,
            }
        );

        manager.EncounterTypes.RegisterEntry(
            new FEncounterType
            {
                ID = FEncounterType.WaterEvening,
                Name = FText.Localized(LocalizationNamespace, "WaterEvening", "WaterEvening"),
                Type = EEncounterTrigger.Water,
                TriggerChance = 2,
            }
        );

        manager.EncounterTypes.RegisterEntry(
            new FEncounterType
            {
                ID = FEncounterType.OldRod,
                Name = FText.Localized(LocalizationNamespace, "OldRod", "OldRod"),
                Type = EEncounterTrigger.Fishing,
            }
        );

        manager.EncounterTypes.RegisterEntry(
            new FEncounterType
            {
                ID = FEncounterType.GoodRod,
                Name = FText.Localized(LocalizationNamespace, "GoodRod", "GoodRod"),
                Type = EEncounterTrigger.Fishing,
            }
        );

        manager.EncounterTypes.RegisterEntry(
            new FEncounterType
            {
                ID = FEncounterType.SuperRod,
                Name = FText.Localized(LocalizationNamespace, "SuperRod", "SuperRod"),
                Type = EEncounterTrigger.Fishing,
            }
        );

        manager.EncounterTypes.RegisterEntry(
            new FEncounterType
            {
                ID = FEncounterType.RockSmash,
                Name = FText.Localized(LocalizationNamespace, "RockSmash", "RockSmash"),
                Type = EEncounterTrigger.None,
                TriggerChance = 50,
            }
        );

        manager.EncounterTypes.RegisterEntry(
            new FEncounterType
            {
                ID = FEncounterType.HeadbuttLow,
                Name = FText.Localized(LocalizationNamespace, "HeadbuttLow", "HeadbuttLow"),
                Type = EEncounterTrigger.None,
            }
        );

        manager.EncounterTypes.RegisterEntry(
            new FEncounterType
            {
                ID = FEncounterType.HeadbuttHigh,
                Name = FText.Localized(LocalizationNamespace, "HeadbuttHigh", "HeadbuttHigh"),
                Type = EEncounterTrigger.None,
            }
        );

        manager.EncounterTypes.RegisterEntry(
            new FEncounterType
            {
                ID = FEncounterType.BugContest,
                Name = FText.Localized(LocalizationNamespace, "BugContest", "BugContest"),
                Type = EEncounterTrigger.Contest,
                TriggerChance = 21,
            }
        );

        return manager;
    }
}
