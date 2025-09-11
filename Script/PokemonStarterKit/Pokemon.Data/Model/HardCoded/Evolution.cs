using GameAccessTools.SourceGenerator.Attributes;
using GameDataAccessTools.Core;
using GameDataAccessTools.Core.Utilities;
using JetBrains.Annotations;
using Pokemon.Data.Model.PBS;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.Attributes.MetaTags;
using UnrealSharp.CoreUObject;
using UnrealSharp.Engine;
using UnrealSharp.GameDataAccessTools;
using UnrealSharp.GameplayTags;
using FMoveHandle = UnrealSharp.PokemonData.FMoveHandle;

namespace Pokemon.Data.Model.HardCoded;

[UStruct]
public readonly partial record struct FIntEvolutionConditionData
{
    [field: UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere)]
    public int Parameter { get; init; }
}

[UStruct]
public readonly partial record struct FMoveEvolutionConditionData
{
    [field: UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere)]
    public FMoveHandle MoveID { get; init; }
}

[UStruct]
public readonly partial record struct FTypeEvolutionConditionData
{
    [field: UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere)]
    public FTypeHandle TypeID { get; init; }
}

[UStruct]
public readonly partial record struct FItemEvolutionConditionData
{
    [field: UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere)]
    public FItemHandle ItemID { get; init; }
}

[UStruct]
public readonly partial record struct FSpeciesEvolutionConditionData
{
    [field: UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere)]
    public FSpeciesHandle SpeciesID { get; init; }
}

[UStruct]
public readonly partial record struct FLocationEvolutionConditionData
{
    [field: UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere)]
    public TSoftObjectPtr<UWorld> Level { get; init; }
}

[UStruct]
public readonly partial record struct FLocationFlagEvolutionConditionData
{
    public const string TagCategory = "Field.Location.Evolution";

    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere)]
    [Categories(TagCategory)]
    public FGameplayTag Flag { get; init; }
}

[UStruct]
[CreateStructView]
public readonly partial struct FEvolution : IGameDataEntry
{
    [UsedImplicitly]
    [field: UProperty(PropertyFlags.BlueprintReadOnly)]
    public required FName ID { get; init; }

    [UsedImplicitly]
    [field: UProperty(PropertyFlags.BlueprintReadOnly)]
    public int RowIndex { get; init; }

    [UsedImplicitly]
    [field: UProperty(PropertyFlags.BlueprintReadOnly)]
    [GameAccessTools.SourceGenerator.Attributes.DisplayName]
    public required FText Name { get; init; }

    [UsedImplicitly]
    [field: UProperty(PropertyFlags.BlueprintReadOnly)]
    public UScriptStruct? Parameter { get; init; }
}

[UClass]
[GameDataRepository<FEvolution>]
[UsedImplicitly]
public partial class UEvolutionRepository : UStaticGameDataRepository;

[UStruct]
[DataHandle(typeof(GameData), nameof(GameData.Evolutions))]
public readonly partial record struct FEvolutionHandle;

public static class EvolutionExtensions
{
    private const string LocalizationNamespace = "GameData.Evolution";

    public static UGameDataManager AddEvolutions(this UGameDataManager manager)
    {
        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.Level,
                Name = FText.Localized(LocalizationNamespace, "Level", "Level"),
                Parameter = FIntEvolutionConditionData.StaticStruct
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.LevelMale,
                Name = FText.Localized(LocalizationNamespace, "LevelMale", "LevelMale"),
                Parameter = FIntEvolutionConditionData.StaticStruct
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.LevelFemale,
                Name = FText.Localized(LocalizationNamespace, "LevelFemale", "LevelFemale"),
                Parameter = FIntEvolutionConditionData.StaticStruct
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.LevelDay,
                Name = FText.Localized(LocalizationNamespace, "LevelDay", "LevelDay"),
                Parameter = FIntEvolutionConditionData.StaticStruct
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.LevelNight,
                Name = FText.Localized(LocalizationNamespace, "LevelNight", "LevelNight"),
                Parameter = FIntEvolutionConditionData.StaticStruct
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.LevelMorning,
                Name = FText.Localized(LocalizationNamespace, "LevelMorning", "LevelMorning"),
                Parameter = FIntEvolutionConditionData.StaticStruct
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.LevelAfternoon,
                Name = FText.Localized(LocalizationNamespace, "LevelAfternoon", "LevelAfternoon"),
                Parameter = FIntEvolutionConditionData.StaticStruct
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.LevelEvening,
                Name = FText.Localized(LocalizationNamespace, "LevelEvening", "LevelEvening"),
                Parameter = FIntEvolutionConditionData.StaticStruct
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.LevelNoWeather,
                Name = FText.Localized(LocalizationNamespace, "LevelNoWeather", "LevelNoWeather"),
                Parameter = FIntEvolutionConditionData.StaticStruct
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.LevelSun,
                Name = FText.Localized(LocalizationNamespace, "LevelSun", "LevelSun"),
                Parameter = FIntEvolutionConditionData.StaticStruct
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.LevelRain,
                Name = FText.Localized(LocalizationNamespace, "LevelRain", "LevelRain"),
                Parameter = FIntEvolutionConditionData.StaticStruct
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.LevelSnow,
                Name = FText.Localized(LocalizationNamespace, "LevelSnow", "LevelSnow"),
                Parameter = FIntEvolutionConditionData.StaticStruct
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.LevelSandstorm,
                Name = FText.Localized(LocalizationNamespace, "LevelSandstorm", "LevelSandstorm"),
                Parameter = FIntEvolutionConditionData.StaticStruct
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.LevelCycling,
                Name = FText.Localized(LocalizationNamespace, "LevelCycling", "LevelCycling"),
                Parameter = FIntEvolutionConditionData.StaticStruct
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.LevelSurfing,
                Name = FText.Localized(LocalizationNamespace, "LevelSurfing", "LevelSurfing"),
                Parameter = FIntEvolutionConditionData.StaticStruct
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.LevelDiving,
                Name = FText.Localized(LocalizationNamespace, "LevelDiving", "LevelDiving"),
                Parameter = FIntEvolutionConditionData.StaticStruct
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.LevelDarkness,
                Name = FText.Localized(LocalizationNamespace, "LevelDarkness", "LevelDarkness"),
                Parameter = FIntEvolutionConditionData.StaticStruct
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.LevelDarkInParty,
                Name = FText.Localized(LocalizationNamespace, "LevelDarkInParty", "LevelDarkInParty"),
                Parameter = FIntEvolutionConditionData.StaticStruct
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.AttackGreater,
                Name = FText.Localized(LocalizationNamespace, "AttackGreater", "AttackGreater"),
                Parameter = FIntEvolutionConditionData.StaticStruct
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.AtkDefEqual,
                Name = FText.Localized(LocalizationNamespace, "AtkDefEqual", "AtkDefEqual"),
                Parameter = FIntEvolutionConditionData.StaticStruct
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.DefenseGreater,
                Name = FText.Localized(LocalizationNamespace, "DefenseGreater", "DefenseGreater"),
                Parameter = FIntEvolutionConditionData.StaticStruct
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.Silcoon,
                Name = FText.Localized(LocalizationNamespace, "Silcoon", "Silcoon"),
                Parameter = FIntEvolutionConditionData.StaticStruct
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.Cascoon,
                Name = FText.Localized(LocalizationNamespace, "Cascoon", "Cascoon"),
                Parameter = FIntEvolutionConditionData.StaticStruct
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.Ninjask,
                Name = FText.Localized(LocalizationNamespace, "Ninjask", "Ninjask"),
                Parameter = FIntEvolutionConditionData.StaticStruct
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.Shedinja,
                Name = FText.Localized(LocalizationNamespace, "Shedinja", "Shedinja"),
                Parameter = FIntEvolutionConditionData.StaticStruct
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
                Parameter = FMoveEvolutionConditionData.StaticStruct
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.HappinessMoveType,
                Name = FText.Localized(LocalizationNamespace, "HappinessMoveType", "HappinessMoveType"),
                Parameter = FTypeEvolutionConditionData.StaticStruct
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.HappinessHoldItem,
                Name = FText.Localized(LocalizationNamespace, "HappinessHoldItem", "HappinessHoldItem"),
                Parameter = FItemEvolutionConditionData.StaticStruct
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.MaxHappiness,
                Name = FText.Localized(LocalizationNamespace, "MaxHappiness", "MaxHappiness")
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.Beauty,
                Name = FText.Localized(LocalizationNamespace, "Beauty", "Beauty"),
                Parameter = FIntEvolutionConditionData.StaticStruct
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.HoldItem,
                Name = FText.Localized(LocalizationNamespace, "HoldItem", "HoldItem"),
                Parameter = FItemEvolutionConditionData.StaticStruct
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.HoldItemMale,
                Name = FText.Localized(LocalizationNamespace, "HoldItemMale", "HoldItemMale"),
                Parameter = FItemEvolutionConditionData.StaticStruct
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.HoldItemFemale,
                Name = FText.Localized(LocalizationNamespace, "HoldItemFemale", "HoldItemFemale"),
                Parameter = FItemEvolutionConditionData.StaticStruct
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.DayHoldItem,
                Name = FText.Localized(LocalizationNamespace, "DayHoldItem", "DayHoldItem"),
                Parameter = FItemEvolutionConditionData.StaticStruct
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.NightHoldItem,
                Name = FText.Localized(LocalizationNamespace, "NightHoldItem", "NightHoldItem"),
                Parameter = FItemEvolutionConditionData.StaticStruct
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.HoldItemHappiness,
                Name = FText.Localized(LocalizationNamespace, "HoldItemHappiness", "HoldItemHappiness"),
                Parameter = FItemEvolutionConditionData.StaticStruct
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.HasMove,
                Name = FText.Localized(LocalizationNamespace, "HasMove", "HasMove"),
                Parameter = FMoveEvolutionConditionData.StaticStruct
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.HasMoveType,
                Name = FText.Localized(LocalizationNamespace, "HasMoveType", "HasMoveType"),
                Parameter = FTypeEvolutionConditionData.StaticStruct
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.HasInParty,
                Name = FText.Localized(LocalizationNamespace, "HasInParty", "HasInParty"),
                Parameter = FSpeciesEvolutionConditionData.StaticStruct
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.Location,
                Name = FText.Localized(LocalizationNamespace, "Location", "Location"),
                Parameter = FLocationEvolutionConditionData.StaticStruct
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.LocationFlag,
                Name = FText.Localized(LocalizationNamespace, "LocationFlag", "LocationFlag"),
                Parameter = FLocationFlagEvolutionConditionData.StaticStruct
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.Region,
                Name = FText.Localized(LocalizationNamespace, "Region", "Region"),
                Parameter = FIntEvolutionConditionData.StaticStruct
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.Item,
                Name = FText.Localized(LocalizationNamespace, "Item", "Item"),
                Parameter = FItemEvolutionConditionData.StaticStruct
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.ItemMale,
                Name = FText.Localized(LocalizationNamespace, "ItemMale", "ItemMale"),
                Parameter = FItemEvolutionConditionData.StaticStruct
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.ItemFemale,
                Name = FText.Localized(LocalizationNamespace, "ItemFemale", "ItemFemale"),
                Parameter = FItemEvolutionConditionData.StaticStruct
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.ItemDay,
                Name = FText.Localized(LocalizationNamespace, "ItemDay", "ItemDay"),
                Parameter = FItemEvolutionConditionData.StaticStruct
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.ItemNight,
                Name = FText.Localized(LocalizationNamespace, "ItemNight", "ItemNight"),
                Parameter = FItemEvolutionConditionData.StaticStruct
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.ItemHappiness,
                Name = FText.Localized(LocalizationNamespace, "ItemHappiness", "ItemHappiness"),
                Parameter = FItemEvolutionConditionData.StaticStruct
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.Trade,
                Name = FText.Localized(LocalizationNamespace, "Trade", "Trade")
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.TradeMale,
                Name = FText.Localized(LocalizationNamespace, "TradeMale", "TradeMale")
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.TradeFemale,
                Name = FText.Localized(LocalizationNamespace, "TradeFemale", "TradeFemale")
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.TradeDay,
                Name = FText.Localized(LocalizationNamespace, "TradeDay", "TradeDay")
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.TradeNight,
                Name = FText.Localized(LocalizationNamespace, "TradeNight", "TradeNight")
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.TradeItem,
                Name = FText.Localized(LocalizationNamespace, "TradeItem", "TradeItem"),
                Parameter = FItemEvolutionConditionData.StaticStruct
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.TradeSpecies,
                Name = FText.Localized(LocalizationNamespace, "TradeSpecies", "TradeSpecies"),
                Parameter = FSpeciesEvolutionConditionData.StaticStruct
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.BattleDealCriticalHit,
                Name = FText.Localized(LocalizationNamespace, "BattleDealCriticalHit", "BattleDealCriticalHit"),
                Parameter = FIntEvolutionConditionData.StaticStruct
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.Event,
                Name = FText.Localized(LocalizationNamespace, "Event", "Event"),
                Parameter = FIntEvolutionConditionData.StaticStruct
            }
        );

        manager.Evolutions.RegisterEntry(
            new FEvolution
            {
                ID = FEvolution.EventAfterDamageTaken,
                Name = FText.Localized(LocalizationNamespace, "EventAfterDamageTaken", "EventAfterDamageTaken"),
                Parameter = FIntEvolutionConditionData.StaticStruct
            }
        );

        return manager;
    }
}
