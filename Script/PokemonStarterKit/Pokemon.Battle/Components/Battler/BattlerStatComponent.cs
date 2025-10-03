using Pokemon.Battle.Entities;
using Pokemon.Battle.Subsystems;
using Pokemon.Data.Model.HardCoded;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.RPGCore;

namespace Pokemon.Battle.Components.Battler;

/// <summary>
/// UBattlerStatComponent represents a component that manages the statistical properties
/// and attributes of a battler in the game. It extends the functionality of URPGComponent
/// and contains battler-specific stats such as Level, HP, Attack, Defense, and others.
/// This component interacts with the game system to handle battler stats and provides utility
/// properties and methods for stat-based calculations.
/// </summary>
[UClass]
public class UBattlerStatComponent : URPGComponent
{
    [UProperty(PropertyFlags.Transient)]
    private UBattlerSubsystem BattlerSubsystem { get; set; }

    /// <summary>
    /// Gets or sets the level of the battler. This property represents the current level
    /// of the battler within the game and is used to determine combat performance, experience tracking,
    /// and progression mechanics.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadWrite, Category = "Battler|Stats")]
    public int Level { get; set; }

    /// <summary>
    /// Gets or sets the current HP (Hit Points) of the battler. This property represents the
    /// battler's current state of health during the game. It is dynamically adjusted based on
    /// damage calculations, healing effects, or other game mechanics and determines whether
    /// the battler is active or has fainted.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadWrite, Category = "Battler|Stats")]
    public int HP { get; set; }

    /// <summary>
    /// Gets or sets the maximum hit points (HP) of the battler. This property determines the
    /// highest possible health value the battler can have during the game. It directly affects
    /// the battler's survivability in combat scenarios and may be influenced by stats, level,
    /// or abilities.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadWrite, Category = "Battler|Stats")]
    public int MaxHP { get; set; }

    /// <summary>
    /// Gets or sets the attack stat of the battler. This property represents the battler's
    /// raw physical offensive power and is used in calculations for damage dealt with
    /// physical attacks in combat scenarios.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly, Category = "Battler|Stats")]
    public int Attack { get; set; }

    /// <summary>
    /// Gets or sets the defense stat of the battler. This property represents the battler's
    /// ability to mitigate incoming physical damage during battles. Higher values indicate
    /// better resistance to physical attacks.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly, Category = "Battler|Stats")]
    public int Defense { get; set; }

    /// <summary>
    /// Gets or sets the Special Attack stat of the battler. This property represents the battler's
    /// ability to execute special type moves, which typically scale with this stat to determine
    /// their effectiveness in combat.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadWrite, Category = "Battler|Stats")]
    public int SpecialAttack { get; set; }

    /// <summary>
    /// Gets or sets the Special Defense stat of the battler. This property determines the
    /// battler's resistance to special attacks during battles, impacting the effectiveness
    /// of incoming special moves in the game's combat mechanics.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadWrite, Category = "Battler|Stats")]
    public int SpecialDefense { get; set; }

    /// <summary>
    /// Gets or sets the speed of the battler. This property represents the base speed
    /// attribute, which influences the battler's action priority, turn order, and interactions
    /// during a battle.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadWrite, Category = "Battler|Stats")]
    public int Speed { get; set; }

    [UProperty]
    private TMap<FBattleStatHandle, int> StatStages { get; }

    /// <summary>
    /// Gets the effective speed of the battler. This property represents the calculated speed
    /// which includes modifications from status effects, abilities, and other battle conditions,
    /// providing the actual speed used during gameplay in battles.
    /// </summary>
    public int EffectiveSpeed => BattlerSubsystem.BattleStatsService.GetEffectiveSpeed((UBattler)OwningEntity);

    /// <summary>
    /// Gets a value indicating whether the battler is fainted. This property evaluates to true
    /// if the battler's current HP is zero, signifying that the battler is unable to continue
    /// participating in the battle.
    /// </summary>
    public bool IsFainted => HP == 0;

    /// <inheritdoc />
    protected override void PreInitialize()
    {
        BattlerSubsystem = GetGameInstanceSubsystem<UBattlerSubsystem>();
    }

    /// <summary>
    /// Retrieves the stage of a specific battle stat for the current battler.
    /// </summary>
    /// <param name="handle">The handle representing the specific battle stat whose stage is to be retrieved.</param>
    /// <returns>The integer value of the stat stage associated with the given handle. If the stat stage is not found, returns the default value of 0.</returns>
    public int GetStatStages(FBattleStatHandle handle)
    {
        return StatStages.GetValueOrDefault(handle);
    }

    /// <summary>
    /// Determines whether the stat stage for the specified battle stat handle has reached its maximum value.
    /// </summary>
    /// <param name="handle">The handle representing the battle stat to query.</param>
    /// <returns>Returns true if the stat stage is at its maximum value; otherwise, false.</returns>
    public bool StatStageAtMax(FBattleStatHandle handle)
    {
        return GetStatStages(handle) >= BattlerSubsystem.BattleStatsService.StatStageMaximum;
    }
}
