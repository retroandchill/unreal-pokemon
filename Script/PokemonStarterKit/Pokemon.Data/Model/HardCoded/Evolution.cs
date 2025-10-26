using GameAccessTools.SourceGenerator.Attributes;
using GameDataAccessTools.Core;
using GameDataAccessTools.Core.Utilities;
using JetBrains.Annotations;
using Pokemon.Data.Model.PBS;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.Attributes.MetaTags;
using UnrealSharp.Core;
using UnrealSharp.CoreUObject;
using UnrealSharp.Engine;
using UnrealSharp.GameDataAccessTools;
using UnrealSharp.GameplayTags;

namespace Pokemon.Data.Model.HardCoded;

/// <summary>
/// Represents evolution condition data based on an integer parameter.
/// </summary>
/// <remarks>
/// This structure is used in the context of handling evolution conditions defined by a single integer.
/// </remarks>
[UStruct]
public readonly partial record struct FIntEvolutionConditionData
{
    /// <summary>
    /// Represents a parameter associated with an evolution.
    /// </summary>
    /// <remarks>
    /// This property holds a reference to a script structure (`UScriptStruct`) that serves as a
    /// flexible parameter for defining evolution conditions. The exact structure of the parameter
    /// is determined by its usage context.
    /// </remarks>
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere)]
    public int Parameter { get; init; }
}

/// <summary>
/// Represents evolution condition data based on a specific move.
/// </summary>
/// <remarks>
/// This structure is used to manage evolution conditions related to a move, identified by its handle.
/// </remarks>
[UStruct]
public readonly partial record struct FMoveEvolutionConditionData
{
    /// <summary>
    /// Represents a reference to a specific move by its handle.
    /// </summary>
    /// <remarks>
    /// This property is used to identify a move within the game data system. It is primarily utilized
    /// for evolution conditions or other gameplay mechanics that depend on a particular move.
    /// </remarks>
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere)]
    public FMoveHandle MoveID { get; init; }
}

/// <summary>
/// Represents evolution condition data associated with a specific type parameter.
/// </summary>
/// <remarks>
/// This structure is utilized to define evolution conditions that depend on the type information within a Pokémon evolution system.
/// </remarks>
[UStruct]
public readonly partial record struct FTypeEvolutionConditionData
{
    /// <summary>
    /// Represents a unique identifier for a type within the game data system.
    /// </summary>
    /// <remarks>
    /// This property serves as a handle to reference specific type information defined
    /// within the Pokémon data model. It is used in conjunction with other properties or systems
    /// that depend on type-related evolution conditions.
    /// </remarks>
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere)]
    public FTypeHandle TypeID { get; init; }
}

/// <summary>
/// Represents evolution condition data based on an item.
/// </summary>
/// <remarks>
/// This structure is utilized in scenarios where evolution conditions are associated with a specific item.
/// </remarks>
[UStruct]
public readonly partial record struct FItemEvolutionConditionData
{
    /// <summary>
    /// Identifies the item associated with an evolution condition.
    /// </summary>
    /// <remarks>
    /// This property references a specific item used to define the conditions for an evolution.
    /// The item is encapsulated by an `FItemHandle`, which links to the relevant item data.
    /// </remarks>
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere)]
    public FItemHandle ItemID { get; init; }
}

/// <summary>
/// Represents evolution condition data linked to a specific species.
/// </summary>
/// <remarks>
/// This structure is utilized to define the evolution conditions based on a related species identifier.
/// </remarks>
[UStruct]
public readonly partial record struct FSpeciesEvolutionConditionData
{
    /// <summary>
    /// Represents the identifier for a specific Pokémon species.
    /// </summary>
    /// <remarks>
    /// This property serves as a handle to uniquely reference a species
    /// within the game data, linking evolution conditions to the relevant
    /// species entry.
    /// </remarks>
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere)]
    public FSpeciesHandle SpeciesID { get; init; }
}

/// <summary>
/// Represents evolution condition data based on a specific location within a game world.
/// </summary>
/// <remarks>
/// This structure is used to define evolution conditions tied to a particular game level or environment.
/// </remarks>
[UStruct]
public readonly partial record struct FLocationEvolutionConditionData
{
    /// <summary>
    /// Represents the game level or environment associated with a location-based evolution condition.
    /// </summary>
    /// <remarks>
    /// This property holds a reference to a specific game world level, represented as a <see cref="TSoftObjectPtr{UWorld}"/>,
    /// which serves as the basis for determining whether an evolution condition tied to location is met.
    /// </remarks>
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere)]
    public TSoftObjectPtr<UWorld> Level { get; init; }
}

/// <summary>
/// Represents evolution condition data based on a specific location flag.
/// </summary>
/// <remarks>
/// This structure is utilized to manage and validate evolution conditions tied to a defined location flag.
/// </remarks>
[UStruct]
public readonly partial record struct FLocationFlagEvolutionConditionData
{
    /// <summary>
    /// Represents the tag category associated with location-based evolution conditions.
    /// </summary>
    /// <remarks>
    /// This constant defines the category identifier used for organizing and managing gameplay tags
    /// specific to location-related evolution conditions in the context of game data.
    /// </remarks>
    public const string TagCategory = "Field.Location.Evolution";

    /// <summary>
    /// Represents a gameplay tag used to define a specific location flag for evolution conditions.
    /// </summary>
    /// <remarks>
    /// This property specifies a gameplay tag that identifies location-based criteria essential for triggering certain evolution conditions.
    /// </remarks>
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere)]
    [Categories(TagCategory)]
    public FGameplayTag Flag { get; init; }
}

/// <summary>
/// Represents an evolution entry within the game data.
/// </summary>
/// <remarks>
/// This structure includes relevant details about a specific evolution, such as its identifier,
/// name, associated parameter, and row index within the data.
/// </remarks>
[UStruct]
[CreateStructView]
public readonly partial struct FEvolution : IGameDataEntry
{
    /// <inheritdoc />
    [UProperty(PropertyFlags.BlueprintReadOnly)]
    public required FName ID { get; init; }

    /// <inheritdoc />
    [UProperty(PropertyFlags.BlueprintReadOnly)]
    public int RowIndex { get; init; }

    /// <summary>
    /// Represents the name of an evolution entity.
    /// </summary>
    /// <remarks>
    /// This property holds a localized text (`FText`) that is intended to display the name or label
    /// of the associated evolution structure. It is annotated for in-game representation purposes.
    /// </remarks>
    [UProperty(PropertyFlags.BlueprintReadOnly)]
    [GameAccessTools.SourceGenerator.Attributes.DisplayName]
    public required FText Name { get; init; }

    /// <summary>
    /// Defines a parameter utilized in evolution conditions or mechanics.
    /// </summary>
    /// <remarks>
    /// This property encapsulates a `UScriptStruct`, which provides a modular and customizable
    /// way to parameterize specific aspects of evolution logic. The structure varies based on
    /// the implementation context and the desired behavior.
    /// </remarks>
    [UProperty(PropertyFlags.BlueprintReadOnly)]
    public UScriptStruct? Parameter { get; init; }
}

/// <summary>
/// Represents a repository for managing evolution-related game data.
/// </summary>
/// <remarks>
/// This class is a specialized static game data repository designed to handle and provide access to evolution data
/// within the context of the game. It utilizes the FEvolution data structure and adheres to the framework's
/// repository model for handling game data efficiently.
/// </remarks>
[UClass]
[GameDataRepository<FEvolution>]
public partial class UEvolutionRepository : UStaticGameDataRepository;

/// <summary>
/// Represents a handle to reference evolution-related data.
/// </summary>
/// <remarks>
/// This structure is used in the context of accessing and managing evolution data within the game framework.
/// It acts as a data handle tied to the evolution repository.
/// </remarks>
[DataHandle(typeof(GameData), nameof(GameData.Evolutions))]
public readonly partial record struct FEvolutionHandle;

/// <summary>
/// Provides extension methods for handling evolution-related functionality in the context of game data management.
/// </summary>
/// <remarks>
/// This static class contains methods for augmenting and managing evolution data within the game data manager.
/// </remarks>
public static class EvolutionExtensions
{
    private const string LocalizationNamespace = "GameData.Evolution";

    /// <summary>
    /// Adds evolutions to the UGameDataManager instance.
    /// </summary>
    /// <param name="manager">The UGameDataManager instance to which the evolutions will be added.</param>
    /// <returns>The UGameDataManager instance with the added evolutions.</returns>
    public static UGameDataManager AddEvolutions(this UGameDataManager manager)
    {
        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.Level,
                Name = FText.Localized(LocalizationNamespace, "Level", "Level"),
                Parameter = FIntEvolutionConditionData.StaticStruct,
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.LevelMale,
                Name = FText.Localized(LocalizationNamespace, "LevelMale", "LevelMale"),
                Parameter = FIntEvolutionConditionData.StaticStruct,
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.LevelFemale,
                Name = FText.Localized(LocalizationNamespace, "LevelFemale", "LevelFemale"),
                Parameter = FIntEvolutionConditionData.StaticStruct,
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.LevelDay,
                Name = FText.Localized(LocalizationNamespace, "LevelDay", "LevelDay"),
                Parameter = FIntEvolutionConditionData.StaticStruct,
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.LevelNight,
                Name = FText.Localized(LocalizationNamespace, "LevelNight", "LevelNight"),
                Parameter = FIntEvolutionConditionData.StaticStruct,
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.LevelMorning,
                Name = FText.Localized(LocalizationNamespace, "LevelMorning", "LevelMorning"),
                Parameter = FIntEvolutionConditionData.StaticStruct,
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.LevelAfternoon,
                Name = FText.Localized(LocalizationNamespace, "LevelAfternoon", "LevelAfternoon"),
                Parameter = FIntEvolutionConditionData.StaticStruct,
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.LevelEvening,
                Name = FText.Localized(LocalizationNamespace, "LevelEvening", "LevelEvening"),
                Parameter = FIntEvolutionConditionData.StaticStruct,
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.LevelNoWeather,
                Name = FText.Localized(LocalizationNamespace, "LevelNoWeather", "LevelNoWeather"),
                Parameter = FIntEvolutionConditionData.StaticStruct,
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.LevelSun,
                Name = FText.Localized(LocalizationNamespace, "LevelSun", "LevelSun"),
                Parameter = FIntEvolutionConditionData.StaticStruct,
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.LevelRain,
                Name = FText.Localized(LocalizationNamespace, "LevelRain", "LevelRain"),
                Parameter = FIntEvolutionConditionData.StaticStruct,
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.LevelSnow,
                Name = FText.Localized(LocalizationNamespace, "LevelSnow", "LevelSnow"),
                Parameter = FIntEvolutionConditionData.StaticStruct,
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.LevelSandstorm,
                Name = FText.Localized(LocalizationNamespace, "LevelSandstorm", "LevelSandstorm"),
                Parameter = FIntEvolutionConditionData.StaticStruct,
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.LevelCycling,
                Name = FText.Localized(LocalizationNamespace, "LevelCycling", "LevelCycling"),
                Parameter = FIntEvolutionConditionData.StaticStruct,
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.LevelSurfing,
                Name = FText.Localized(LocalizationNamespace, "LevelSurfing", "LevelSurfing"),
                Parameter = FIntEvolutionConditionData.StaticStruct,
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.LevelDiving,
                Name = FText.Localized(LocalizationNamespace, "LevelDiving", "LevelDiving"),
                Parameter = FIntEvolutionConditionData.StaticStruct,
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.LevelDarkness,
                Name = FText.Localized(LocalizationNamespace, "LevelDarkness", "LevelDarkness"),
                Parameter = FIntEvolutionConditionData.StaticStruct,
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.LevelDarkInParty,
                Name = FText.Localized(LocalizationNamespace, "LevelDarkInParty", "LevelDarkInParty"),
                Parameter = FIntEvolutionConditionData.StaticStruct,
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.AttackGreater,
                Name = FText.Localized(LocalizationNamespace, "AttackGreater", "AttackGreater"),
                Parameter = FIntEvolutionConditionData.StaticStruct,
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.AtkDefEqual,
                Name = FText.Localized(LocalizationNamespace, "AtkDefEqual", "AtkDefEqual"),
                Parameter = FIntEvolutionConditionData.StaticStruct,
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.DefenseGreater,
                Name = FText.Localized(LocalizationNamespace, "DefenseGreater", "DefenseGreater"),
                Parameter = FIntEvolutionConditionData.StaticStruct,
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.Silcoon,
                Name = FText.Localized(LocalizationNamespace, "Silcoon", "Silcoon"),
                Parameter = FIntEvolutionConditionData.StaticStruct,
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.Cascoon,
                Name = FText.Localized(LocalizationNamespace, "Cascoon", "Cascoon"),
                Parameter = FIntEvolutionConditionData.StaticStruct,
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.Ninjask,
                Name = FText.Localized(LocalizationNamespace, "Ninjask", "Ninjask"),
                Parameter = FIntEvolutionConditionData.StaticStruct,
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.Shedinja,
                Name = FText.Localized(LocalizationNamespace, "Shedinja", "Shedinja"),
                Parameter = FIntEvolutionConditionData.StaticStruct,
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.Happiness,
                Name = FText.Localized(LocalizationNamespace, "Happiness", "Happiness"),
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.HappinessMale,
                Name = FText.Localized(LocalizationNamespace, "HappinessMale", "HappinessMale"),
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.HappinessFemale,
                Name = FText.Localized(LocalizationNamespace, "HappinessFemale", "HappinessFemale"),
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.HappinessDay,
                Name = FText.Localized(LocalizationNamespace, "HappinessDay", "HappinessDay"),
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.HappinessNight,
                Name = FText.Localized(LocalizationNamespace, "HappinessNight", "HappinessNight"),
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.HappinessMove,
                Name = FText.Localized(LocalizationNamespace, "HappinessMove", "HappinessMove"),
                Parameter = FMoveEvolutionConditionData.StaticStruct,
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.HappinessMoveType,
                Name = FText.Localized(LocalizationNamespace, "HappinessMoveType", "HappinessMoveType"),
                Parameter = FTypeEvolutionConditionData.StaticStruct,
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.HappinessHoldItem,
                Name = FText.Localized(LocalizationNamespace, "HappinessHoldItem", "HappinessHoldItem"),
                Parameter = FItemEvolutionConditionData.StaticStruct,
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.MaxHappiness,
                Name = FText.Localized(LocalizationNamespace, "MaxHappiness", "MaxHappiness"),
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.Beauty,
                Name = FText.Localized(LocalizationNamespace, "Beauty", "Beauty"),
                Parameter = FIntEvolutionConditionData.StaticStruct,
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.HoldItem,
                Name = FText.Localized(LocalizationNamespace, "HoldItem", "HoldItem"),
                Parameter = FItemEvolutionConditionData.StaticStruct,
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.HoldItemMale,
                Name = FText.Localized(LocalizationNamespace, "HoldItemMale", "HoldItemMale"),
                Parameter = FItemEvolutionConditionData.StaticStruct,
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.HoldItemFemale,
                Name = FText.Localized(LocalizationNamespace, "HoldItemFemale", "HoldItemFemale"),
                Parameter = FItemEvolutionConditionData.StaticStruct,
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.DayHoldItem,
                Name = FText.Localized(LocalizationNamespace, "DayHoldItem", "DayHoldItem"),
                Parameter = FItemEvolutionConditionData.StaticStruct,
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.NightHoldItem,
                Name = FText.Localized(LocalizationNamespace, "NightHoldItem", "NightHoldItem"),
                Parameter = FItemEvolutionConditionData.StaticStruct,
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.HoldItemHappiness,
                Name = FText.Localized(LocalizationNamespace, "HoldItemHappiness", "HoldItemHappiness"),
                Parameter = FItemEvolutionConditionData.StaticStruct,
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.HasMove,
                Name = FText.Localized(LocalizationNamespace, "HasMove", "HasMove"),
                Parameter = FMoveEvolutionConditionData.StaticStruct,
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.HasMoveType,
                Name = FText.Localized(LocalizationNamespace, "HasMoveType", "HasMoveType"),
                Parameter = FTypeEvolutionConditionData.StaticStruct,
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.HasInParty,
                Name = FText.Localized(LocalizationNamespace, "HasInParty", "HasInParty"),
                Parameter = FSpeciesEvolutionConditionData.StaticStruct,
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.Location,
                Name = FText.Localized(LocalizationNamespace, "Location", "Location"),
                Parameter = FLocationEvolutionConditionData.StaticStruct,
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.LocationFlag,
                Name = FText.Localized(LocalizationNamespace, "LocationFlag", "LocationFlag"),
                Parameter = FLocationFlagEvolutionConditionData.StaticStruct,
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.Region,
                Name = FText.Localized(LocalizationNamespace, "Region", "Region"),
                Parameter = FIntEvolutionConditionData.StaticStruct,
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.Item,
                Name = FText.Localized(LocalizationNamespace, "Item", "Item"),
                Parameter = FItemEvolutionConditionData.StaticStruct,
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.ItemMale,
                Name = FText.Localized(LocalizationNamespace, "ItemMale", "ItemMale"),
                Parameter = FItemEvolutionConditionData.StaticStruct,
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.ItemFemale,
                Name = FText.Localized(LocalizationNamespace, "ItemFemale", "ItemFemale"),
                Parameter = FItemEvolutionConditionData.StaticStruct,
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.ItemDay,
                Name = FText.Localized(LocalizationNamespace, "ItemDay", "ItemDay"),
                Parameter = FItemEvolutionConditionData.StaticStruct,
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.ItemNight,
                Name = FText.Localized(LocalizationNamespace, "ItemNight", "ItemNight"),
                Parameter = FItemEvolutionConditionData.StaticStruct,
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.ItemHappiness,
                Name = FText.Localized(LocalizationNamespace, "ItemHappiness", "ItemHappiness"),
                Parameter = FItemEvolutionConditionData.StaticStruct,
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution { ID = FEvolution.Trade, Name = FText.Localized(LocalizationNamespace, "Trade", "Trade") }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.TradeMale,
                Name = FText.Localized(LocalizationNamespace, "TradeMale", "TradeMale"),
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.TradeFemale,
                Name = FText.Localized(LocalizationNamespace, "TradeFemale", "TradeFemale"),
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.TradeDay,
                Name = FText.Localized(LocalizationNamespace, "TradeDay", "TradeDay"),
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.TradeNight,
                Name = FText.Localized(LocalizationNamespace, "TradeNight", "TradeNight"),
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.TradeItem,
                Name = FText.Localized(LocalizationNamespace, "TradeItem", "TradeItem"),
                Parameter = FItemEvolutionConditionData.StaticStruct,
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.TradeSpecies,
                Name = FText.Localized(LocalizationNamespace, "TradeSpecies", "TradeSpecies"),
                Parameter = FSpeciesEvolutionConditionData.StaticStruct,
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.BattleDealCriticalHit,
                Name = FText.Localized(LocalizationNamespace, "BattleDealCriticalHit", "BattleDealCriticalHit"),
                Parameter = FIntEvolutionConditionData.StaticStruct,
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.Event,
                Name = FText.Localized(LocalizationNamespace, "Event", "Event"),
                Parameter = FIntEvolutionConditionData.StaticStruct,
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.EventAfterDamageTaken,
                Name = FText.Localized(LocalizationNamespace, "EventAfterDamageTaken", "EventAfterDamageTaken"),
                Parameter = FIntEvolutionConditionData.StaticStruct,
            }
        );

        return manager;
    }
}
