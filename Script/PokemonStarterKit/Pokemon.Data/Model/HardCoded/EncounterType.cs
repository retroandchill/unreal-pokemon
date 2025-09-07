using GameAccessTools.SourceGenerator.Attributes;
using GameDataAccessTools.Core;
using GameDataAccessTools.Core.Utilities;
using JetBrains.Annotations;
using LanguageExt;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.Attributes.MetaTags;
using UnrealSharp.GameDataAccessTools;
using UnrealSharp.GameplayTags;

namespace Pokemon.Data.Model.HardCoded;

[UEnum]
public enum EEncounterTrigger : byte
{
    None,
    Land,
    Cave,
    Water,
    Fishing,
    Contest,
}

[UStruct]
[CreateStructView]
public readonly partial struct FEncounterType : IGameDataEntry
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
    [Categories("Field.Encounters.Triggers")]
    public required EEncounterTrigger Type { get; init; }

    [UsedImplicitly]
    [field: UProperty(PropertyFlags.BlueprintReadOnly)]
    public Option<int> TriggerChance { get; init; }
}

[UClass]
[GameDataRepository<FEncounterType>]
[UsedImplicitly]
public partial class UEncounterTypeRepository : UStaticGameDataRepository;

[UStruct]
[DataHandle(typeof(GameData), nameof(GameData.EncounterTypes))]
public readonly partial record struct FEncounterTypeHandle;

public static class EncounterTypeExtensions
{
    private const string LocalizationNamespace = "GameData.EncounterType";

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
