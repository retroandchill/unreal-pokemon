using GameAccessTools.SourceGenerator.Attributes;
using GameDataAccessTools.Core;
using GameDataAccessTools.Core.Utilities;
using JetBrains.Annotations;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.GameDataAccessTools;

namespace Pokemon.Data.Model.HardCoded;

[UEnum]
public enum ETargetCount : byte
{
    NoTarget,
    SingleTarget,
    MultipleTargets,
}

[UStruct]
[CreateStructView]
public readonly partial struct FTarget() : IGameDataEntry
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

    [UsedImplicitly]
    [field: UProperty(PropertyFlags.BlueprintReadOnly)]
    public ETargetCount NumTargets { get; init; } = ETargetCount.NoTarget;

    [UsedImplicitly]
    [field: UProperty(PropertyFlags.BlueprintReadOnly)]
    public bool TargetsFoe { get; init; }

    [UsedImplicitly]
    [field: UProperty(PropertyFlags.BlueprintReadOnly)]
    public bool TargetsAll { get; init; }

    [UsedImplicitly]
    [field: UProperty(PropertyFlags.BlueprintReadOnly)]
    public bool AffectsFoeSide { get; init; }

    [UsedImplicitly]
    [field: UProperty(PropertyFlags.BlueprintReadOnly)]
    public bool LongRange { get; init; }
}

[UClass]
[GameDataRepository<FTarget>]
[UsedImplicitly]
public partial class UTargetRepository : UStaticGameDataRepository;

[UStruct]
[DataHandle(typeof(GameData), nameof(GameData.Targets))]
public readonly partial record struct FTargetHandle;

public static class TargetExtensions
{
    private const string LocalizationNamespace = "GameData.Target";

    public static UGameDataManager AddTargets(this UGameDataManager manager)
    {
        manager.Targets.RegisterEntry(
            new FTarget
            {
                ID = FTarget.User,
                Name = FText.Localized(LocalizationNamespace, "User", "User"),
            }
        );

        manager.Targets.RegisterEntry(
            new FTarget
            {
                ID = FTarget.NearAlly,
                Name = FText.Localized(LocalizationNamespace, "NearAlly", "Near Ally"),
                NumTargets = ETargetCount.SingleTarget,
            }
        );

        manager.Targets.RegisterEntry(
            new FTarget
            {
                ID = FTarget.UserOrNearAlly,
                Name = FText.Localized(
                    LocalizationNamespace,
                    "UserOrNearAlly",
                    "User or Near Ally"
                ),
                NumTargets = ETargetCount.SingleTarget,
            }
        );

        manager.Targets.RegisterEntry(
            new FTarget
            {
                ID = FTarget.AllAllies,
                Name = FText.Localized(LocalizationNamespace, "AllAllies", "All Allies"),
                NumTargets = ETargetCount.MultipleTargets,
                TargetsAll = true,
                LongRange = true,
            }
        );

        manager.Targets.RegisterEntry(
            new FTarget
            {
                ID = FTarget.UserAndAllies,
                Name = FText.Localized(LocalizationNamespace, "UserAndAllies", "User and Allies"),
                NumTargets = ETargetCount.MultipleTargets,
                LongRange = true,
            }
        );

        manager.Targets.RegisterEntry(
            new FTarget
            {
                ID = FTarget.NearFoe,
                Name = FText.Localized(LocalizationNamespace, "NearFoe", "Near Foe"),
                NumTargets = ETargetCount.SingleTarget,
                TargetsFoe = true,
            }
        );

        manager.Targets.RegisterEntry(
            new FTarget
            {
                ID = FTarget.RandomNearFoe,
                Name = FText.Localized(LocalizationNamespace, "RandomNearFoe", "Random Near Foe"),
                NumTargets = ETargetCount.SingleTarget,
                TargetsFoe = true,
            }
        );

        manager.Targets.RegisterEntry(
            new FTarget
            {
                ID = FTarget.AllNearFoes,
                Name = FText.Localized(LocalizationNamespace, "AllNearFoes", "All Near Foes"),
                NumTargets = ETargetCount.MultipleTargets,
                TargetsFoe = true,
            }
        );

        manager.Targets.RegisterEntry(
            new FTarget
            {
                ID = FTarget.Foe,
                Name = FText.Localized(LocalizationNamespace, "Foe", "Foe"),
                NumTargets = ETargetCount.SingleTarget,
                TargetsFoe = true,
                LongRange = true,
            }
        );

        manager.Targets.RegisterEntry(
            new FTarget
            {
                ID = FTarget.AllFoes,
                Name = FText.Localized(LocalizationNamespace, "AllFoes", "All Foes"),
                NumTargets = ETargetCount.MultipleTargets,
                TargetsFoe = true,
                LongRange = true,
            }
        );

        manager.Targets.RegisterEntry(
            new FTarget
            {
                ID = FTarget.NearOther,
                Name = FText.Localized(LocalizationNamespace, "NearOther", "Near Other"),
                NumTargets = ETargetCount.SingleTarget,
                TargetsFoe = true,
            }
        );

        manager.Targets.RegisterEntry(
            new FTarget
            {
                ID = FTarget.AllNearOthers,
                Name = FText.Localized(LocalizationNamespace, "AllNearOthers", "All Near Others"),
                NumTargets = ETargetCount.MultipleTargets,
                TargetsFoe = true,
            }
        );

        manager.Targets.RegisterEntry(
            new FTarget
            {
                ID = FTarget.Other,
                Name = FText.Localized(LocalizationNamespace, "Other", "Other"),
                NumTargets = ETargetCount.SingleTarget,
                TargetsFoe = true,
                LongRange = true,
            }
        );

        manager.Targets.RegisterEntry(
            new FTarget
            {
                ID = FTarget.AllBattlers,
                Name = FText.Localized(LocalizationNamespace, "AllBattlers", "All Battlers"),
                NumTargets = ETargetCount.MultipleTargets,
                TargetsFoe = true,
                TargetsAll = true,
                LongRange = true,
            }
        );

        manager.Targets.RegisterEntry(
            new FTarget
            {
                ID = FTarget.UserSide,
                Name = FText.Localized(LocalizationNamespace, "UserSide", "User Side"),
            }
        );

        manager.Targets.RegisterEntry(
            new FTarget
            {
                ID = FTarget.FoeSide,
                Name = FText.Localized(LocalizationNamespace, "FoeSide", "Foe Side"),
                AffectsFoeSide = true,
            }
        );

        manager.Targets.RegisterEntry(
            new FTarget
            {
                ID = FTarget.BothSides,
                Name = FText.Localized(LocalizationNamespace, "BothSides", "Both Sides"),
                AffectsFoeSide = true,
            }
        );

        return manager;
    }
}
