using System.Diagnostics.CodeAnalysis;
using Pokemon.Battle.Entities;
using Pokemon.Battle.Subsystems;
using Pokemon.Data.Model.PBS;
using UnrealSharp.Attributes;
using UnrealSharp.RPGCore;

namespace Pokemon.Battle.Components.Battler;

/// <summary>
/// The <c>UBattlerAbilityComponent</c> class provides functionality for managing and interacting with a battler's abilities within the game.
/// It is primarily used as a component for battler entities, facilitating ability-related logic and ensuring smooth integration
/// with the game's battle subsystem and associated systems.
/// </summary>
[UClass]
public partial class UBattlerAbilityComponent : URPGComponent
{
    [UProperty(PropertyFlags.Transient)]
    private partial UBattlerSubsystem BattlerSubsystem { get; set; }

    /// <summary>
    /// Represents the ability associated with the battler entity.
    /// This property references the handle to the ability resource, allowing for functionality such as checking
    /// the ability's status and interacting with the game's ability system.
    /// </summary>
    /// <remarks>
    /// The ability can be active or inactive depending on the battler's state and specific gameplay conditions.
    /// This property integrates with the broader battle system by enabling fine-grained control over abilities.
    /// </remarks>
    /// <value>
    /// The handle to the ability resource, or <c>null</c> if no ability is assigned.
    /// </value>
    [UProperty(PropertyFlags.BlueprintReadWrite, Category = "Battler|Ability")]
    public partial FAbilityHandle? Ability { get; set; }

    /// <summary>
    /// Indicates whether the battler's ability is currently active.
    /// This property provides a quick and convenient way to check the active state of the battler's ability,
    /// considering various gameplay conditions such as the battler's state or whether the ability should be ignored when fainted.
    /// </summary>
    /// <remarks>
    /// The active state of the ability depends on internal validations performed within the battle system.
    /// This includes checks for the ability being initialized, the battler's state, and the optional consideration of specific abilities.
    /// It integrates tightly with the game's ability management services to ensure accurate status representation.
    /// </remarks>
    /// <value>
    /// <c>true</c> if the battler's ability is active based on the implemented conditions; otherwise, <c>false</c>.
    /// </value>
    [MemberNotNullWhen(true, nameof(Ability))]
    public bool AbilityActive => IsAbilityActive();

    /// <inheritdoc />
    protected override void PreInitialize_Implementation()
    {
        BattlerSubsystem = GetGameInstanceSubsystem<UBattlerSubsystem>();
    }

    /// <summary>
    /// Determines whether the ability associated with the battler is active.
    /// </summary>
    /// <param name="ignoreFainted">When set to true, ignores the fainted status of the battler while determining if the ability is active.</param>
    /// <param name="checkAbility">An optional specific ability to check against. Defaults to null if not specified.</param>
    /// <returns>True if the ability is active based on the specified parameters; otherwise, false.</returns>
    public bool IsAbilityActive(bool ignoreFainted = false, FAbilityHandle checkAbility = default)
    {
        return BattlerSubsystem.BattleAbilityService.HasActiveAbility(
            (UBattler)OwningEntity,
            ignoreFainted,
            checkAbility
        );
    }

    /// <summary>
    /// Determines whether the battler has an active ability matching the specified ability handle.
    /// </summary>
    /// <param name="checkAbility">The ability handle to check against the battler's active ability.</param>
    /// <param name="ignoreFainted">
    /// If set to <c>true</c>, the check will ignore whether the battler is fainted when determining
    /// if the ability is active. Defaults to <c>false</c>.
    /// </param>
    /// <returns>
    /// <c>true</c> if the battler has an active ability matching the specified ability handle; otherwise, <c>false</c>.
    /// </returns>
    public bool HasActiveAbility(FAbilityHandle checkAbility, bool ignoreFainted = false)
    {
        if (!IsAbilityActive(ignoreFainted, checkAbility))
            return false;
        return Ability == checkAbility;
    }
}
