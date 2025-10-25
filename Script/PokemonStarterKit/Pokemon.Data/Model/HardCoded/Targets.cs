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
/// Represents the targeting count for specific actions or effects within a system.
/// </summary>
[UEnum]
public enum ETargetCount : byte
{
    /// <summary>
    /// Move has no target count.
    /// </summary>
    NoTarget,

    /// <summary>
    /// Move hits a single target.
    /// </summary>
    SingleTarget,

    /// <summary>
    /// Move hits multiple targets.
    /// </summary>
    MultipleTargets,
}

/// <summary>
/// Represents a target configuration within the game data model, defining how and what
/// entities are targeted during gameplay.
/// </summary>
[UStruct]
[CreateStructView]
public readonly partial struct FTarget() : IGameDataEntry
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
    /// Represents the display name associated with the target.
    /// </summary>
    [UsedImplicitly]
    [UProperty(PropertyFlags.BlueprintReadOnly)]
    [DisplayName]
    public required FText Name { get; init; }

    /// <summary>
    /// Represents the number of targets, defined as an enumeration of type <see cref="ETargetCount"/>.
    /// </summary>
    /// <remarks>
    /// This property specifies the number of possible targets for a given context, which can be
    /// <c>NoTarget</c>, <c>SingleTarget</c>, or <c>MultipleTargets</c>.
    /// </remarks>
    [UsedImplicitly]
    [UProperty(PropertyFlags.BlueprintReadOnly)]
    public ETargetCount NumTargets { get; init; } = ETargetCount.NoTarget;

    /// <summary>
    /// Indicates whether the target is a foe.
    /// </summary>
    [UsedImplicitly]
    [UProperty(PropertyFlags.BlueprintReadOnly)]
    public bool TargetsFoe { get; init; }

    /// <summary>
    /// Indicates whether the target applies to all possible entities within the applicable context.
    /// Typically used to determine if the effect or action is broad in scope or affects multiple targets.
    /// </summary>
    [UsedImplicitly]
    [UProperty(PropertyFlags.BlueprintReadOnly)]
    public bool TargetsAll { get; init; }

    /// <summary>
    /// Indicates whether the target affects the opposing team's side in a battle context.
    /// This property determines if the associated action, move, or effect applies to all entities on the foe's side,
    /// rather than a single target or the user's side.
    /// </summary>
    [UsedImplicitly]
    [UProperty(PropertyFlags.BlueprintReadOnly)]
    public bool AffectsFoeSide { get; init; }

    /// <summary>
    /// Indicates whether the target has a long-range capability.
    /// This property is read-only and is marked for blueprint accessibility.
    /// </summary>
    [UsedImplicitly]
    [UProperty(PropertyFlags.BlueprintReadOnly)]
    public bool LongRange { get; init; }
}

/// <summary>
/// A repository responsible for managing and accessing target data within the game.
/// Provides functionality to retrieve and handle instances of the target configuration type.
/// </summary>
[UClass]
[GameDataRepository<FTarget>]
[UsedImplicitly]
public partial class UTargetRepository : UStaticGameDataRepository;

/// <summary>
/// Represents a handle to a specific target configuration within the game's data repository,
/// used to reference target entities and their associated behaviors or attributes.
/// </summary>
[UStruct]
[DataHandle(typeof(GameData), nameof(GameData.Targets))]
public readonly partial record struct FTargetHandle;

/// <summary>
/// Provides extension methods for managing target configurations within the game data model.
/// </summary>
public static class TargetExtensions
{
    private const string LocalizationNamespace = "GameData.Target";

    /// <summary>
    /// Adds predefined targets to the given UGameDataManager instance.
    /// </summary>
    /// <param name="manager">The UGameDataManager instance to which the targets will be added.</param>
    /// <returns>Returns the modified UGameDataManager instance with the added targets.</returns>
    public static UGameDataManager AddTargets(this UGameDataManager manager)
    {
        manager.Targets.RegisterEntry(
            new FTarget { ID = FTarget.User, Name = FText.Localized(LocalizationNamespace, "User", "User") }
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
                Name = FText.Localized(LocalizationNamespace, "UserOrNearAlly", "User or Near Ally"),
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
