using GameAccessTools.SourceGenerator.Attributes;
using GameDataAccessTools.Core;
using GameDataAccessTools.Core.Utilities;
using JetBrains.Annotations;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.GameDataAccessTools;

namespace Pokemon.Data.Model.HardCoded;

[UStruct]
[CreateStructView]
public readonly partial struct FEnvironment : IGameDataEntry
{
    [UsedImplicitly]
    [field: UProperty(PropertyFlags.BlueprintReadOnly)]
    public required FName ID { get; init; }

    [UsedImplicitly]
    [field: UProperty(PropertyFlags.BlueprintReadOnly)]
    public int RowIndex { get; init; }

    [UsedImplicitly]
    [field: UProperty(PropertyFlags.BlueprintReadOnly)]
    [DisplayName]
    public required FText Name { get; init; }

    [field: UProperty(PropertyFlags.BlueprintReadOnly)]
    public required FName BattleBase { get; init; }
}

[UClass]
[GameDataRepository<FEnvironment>]
[UsedImplicitly]
public partial class UEnvironmentRepository : UStaticGameDataRepository;

[UStruct]
[DataHandle(typeof(GameData), nameof(GameData.Environments))]
public readonly partial record struct FEnvironmentHandle;

public static class BattleBase
{
    public static readonly FName Grass = "grass";
    public static readonly FName Water = "water";
    public static readonly FName Puddle = "puddle";
    public static readonly FName Sand = "sand";
    public static readonly FName Ice = "ice";
}

public static class EnvironmentExtensions
{
    private const string LocalizationNamespace = "GameData.Environment";

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
