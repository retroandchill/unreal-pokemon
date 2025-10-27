using Pokemon.Battle.Actions;
using Pokemon.Core.Entities;
using Pokemon.Data.Model.PBS;
using UnrealSharp.Attributes;
using UnrealSharp.GameplayTags;

namespace Pokemon.Battle.Effects;

/// <summary>
/// Represents the data associated with the "Bide" effect in a Pokémon battle.
/// </summary>
/// <remarks>
/// The "Bide" effect involves a Pokémon enduring attacks for a number of turns and
/// then retaliating with damage based on the attacks it received.
/// </remarks>
/// <param name="Turns">
/// The number of turns the "Bide" effect has been active.
/// </param>
/// <param name="Damage">
/// The accumulated damage to retaliate with after the "Bide" effect ends.
/// A value of -1 indicates that no damage has been accumulated.
/// </param>
/// <param name="Target">
/// The target associated with the "Bide" effect. This specifies the intended opponent
/// during the effect's activation.
/// </param>
[UStruct]
public readonly partial record struct FBideEffectData(
    [property: UProperty(PropertyFlags.BlueprintReadOnly)] int Turns,
    [property: UProperty(PropertyFlags.BlueprintReadOnly)] int Damage,
    [property: UProperty(PropertyFlags.BlueprintReadOnly)] FActionTarget Target
)
{
    /// <summary>
    /// Represents a default or uninitialized state for an instance of <see cref="FBideEffectData"/>.
    /// </summary>
    /// <remarks>
    /// This static property provides a standardized way to refer to an empty or default value for the "Bide" effect
    /// data structure. It initializes the object with 0 turns, no accumulated damage (-1), and no specific target.
    /// </remarks>
    public static FBideEffectData None => new(0, -1, FActionTarget.None);
}

/// <summary>
/// Represents the data associated with the "Counter" effect in a Pokémon battle.
/// </summary>
/// <remarks>
/// The "Counter" effect allows a Pokémon to retaliate against physical damage it receives during a battle.
/// The damage dealt by the "Counter" effect is based on the amount of physical damage received.
/// </remarks>
/// <param name="Damage">
/// The accumulated damage to retaliate with when the "Counter" effect triggers.
/// A value of -1 indicates that no damage has been accumulated.
/// </param>
/// <param name="Target">
/// The target associated with the "Counter" effect. This specifies the opponent against
/// whom the effect is applied.
/// </param>
[UStruct]
public readonly partial record struct FCounterEffectData(
    [property: UProperty(PropertyFlags.BlueprintReadOnly)] int Damage,
    [property: UProperty(PropertyFlags.BlueprintReadOnly)] FActionTarget Target
)
{
    /// <summary>
    /// Represents a default or uninitialized state for an instance of <see cref="FCounterEffectData"/>.
    /// </summary>
    /// <remarks>
    /// This static property provides a standardized way to refer to an empty or default value
    /// for the "Counter" effect data structure. It initializes the object with no accumulated damage (-1)
    /// and a default target defined by <see cref="FActionTarget.None"/>.
    /// </remarks>
    public static FCounterEffectData None => new(-1, FActionTarget.None);
}

/// <summary>
/// Represents the data associated with the "Destiny Bond" effect in a Pokémon battle.
/// </summary>
/// <remarks>
/// The "Destiny Bond" effect links the Pokémon to its opponent, causing the opponent
/// to faint if the user's Pokémon is fainted by the opponent during the same turn. This
/// struct tracks whether the effect is currently active, its previous state, and the
/// opponent bound by the effect.
/// </remarks>
/// <param name="Active">
/// Indicates whether the "Destiny Bond" effect is currently active.
/// </param>
/// <param name="Previous">
/// Indicates whether the "Destiny Bond" effect was active during the previous turn.
/// </param>
/// <param name="Target">
/// The target associated with the "Destiny Bond" effect. This specifies the opponent
/// affected when the effect is activated.
/// </param>
[UStruct]
public readonly partial record struct FDestinyBondEffectData(
    [property: UProperty(PropertyFlags.BlueprintReadOnly)] bool Active,
    [property: UProperty(PropertyFlags.BlueprintReadOnly)] bool Previous,
    [property: UProperty(PropertyFlags.BlueprintReadOnly)] FActionTarget Target
)
{
    /// <summary>
    /// Represents a default or uninitialized state for an instance of <see cref="FDestinyBondEffectData"/>.
    /// </summary>
    /// <remarks>
    /// This property provides a standard way to reference a default or inactive "Destiny Bond" effect.
    /// It initializes the effect as inactive, with no effect during the previous turn, and no specific target bound.
    /// </remarks>
    public static FDestinyBondEffectData None => new(false, false, FActionTarget.None);
}

/// <summary>
/// Represents the data associated with a turn-based move effect in a Pokémon battle.
/// </summary>
/// <remarks>
/// This effect tracks the number of turns associated with a specific move and
/// provides details about the move being affected during these turns.
/// It is commonly used for scenarios like disabling or locking a move for a duration.
/// </remarks>
/// <param name="Turns">
/// The number of turns the move effect is active. This can determine the duration
/// for which the effect applies to the move.
/// </param>
/// <param name="Move">
/// The move handle associated with the effect. This indicates the move impacted
/// by the turn-based effect.
/// </param>
[UStruct]
public readonly partial record struct FMoveTurnEffectData(
    [property: UProperty(PropertyFlags.BlueprintReadOnly)] int Turns,
    [property: UProperty(PropertyFlags.BlueprintReadOnly)] FMoveHandle Move
);

/// <summary>
/// Represents the data associated with turn-based effects targeting a specific Pokémon during a battle.
/// </summary>
/// <remarks>
/// This structure encapsulates information about effects that persist for a specific number of turns
/// and are directed at a specific target in a Pokémon battle.
/// </remarks>
/// <param name="Turns">
/// The number of turns the effect remains active. A value of 0 indicates no effect is currently active.
/// </param>
/// <param name="Target">
/// The target Pokémon or participant associated with the effect. This defines which entity the effect is applied to.
/// </param>
[UStruct]
public readonly partial record struct FTargetTurnEffectData(
    [property: UProperty(PropertyFlags.BlueprintReadOnly)] int Turns,
    [property: UProperty(PropertyFlags.BlueprintReadOnly)] FActionTarget Target
)
{
    /// <summary>
    /// Provides a default, uninitialized instance of the <see cref="FTargetTurnEffectData"/> structure.
    /// </summary>
    /// <remarks>
    /// This static property represents a standardized, empty state for the target turn effect data in a battle.
    /// It initializes the effect with 0 remaining turns and an unassigned target, effectively representing
    /// the absence of any active effect.
    /// </remarks>
    public static FTargetTurnEffectData None => new(0, FActionTarget.None);
}

/// <summary>
/// Represents the data associated with the "Illusion" effect in a Pokémon battle.
/// </summary>
/// <remarks>
/// The "Illusion" effect involves a Pokémon taking on the visual appearance of another Pokémon
/// until certain conditions are met, such as taking damage or executing specific actions.
/// </remarks>
/// <param name="Pokemon">
/// The Pokémon currently associated with the "Illusion" effect. If this value is null,
/// the "Illusion" effect is considered invalid.
/// </param>
[UStruct]
public readonly partial record struct FIllusionEffectData(
    [property: UProperty(PropertyFlags.BlueprintReadOnly)] UPokemon? Pokemon
)
{
    /// <summary>
    /// Determines whether the "Illusion" effect is valid for the associated Pokémon.
    /// </summary>
    /// <remarks>
    /// This property checks whether the "Illusion" effect is properly initialized by ensuring
    /// that a non-null Pokémon is associated with it. If no valid Pokémon is associated,
    /// the effect is considered invalid.
    /// </remarks>
    public bool IsValid => Pokemon is not null;
}

/// <summary>
/// Represents the data associated with the "Pickup" effect in a Pokémon battle.
/// </summary>
/// <remarks>
/// The "Pickup" effect involves a Pokémon acquiring an item during battle. This
/// data structure tracks the item acquired and the number of times the effect
/// has been triggered or utilized.
/// </remarks>
/// <param name="Item">
/// The handle representing the item acquired by the Pokémon as a result of the "Pickup"
/// effect.
/// </param>
/// <param name="Use">
/// The number of times the acquired item has been subjected to use or interaction
/// within the context of this effect.
/// </param>
[UStruct]
public readonly partial record struct FPickupEffectData(
    [property: UProperty(PropertyFlags.BlueprintReadOnly)] FItemHandle Item,
    [property: UProperty(PropertyFlags.BlueprintReadOnly)] int Use
);

/// <summary>
/// Represents the data associated with the "Protect" effect in a Pokémon battle.
/// </summary>
/// <remarks>
/// The "Protect" effect allows a Pokémon to shield itself from attacks for a single turn.
/// The effectiveness of using "Protect" consecutively is reduced by a dynamic rate.
/// </remarks>
/// <param name="Active">
/// Indicates whether the "Protect" effect is currently active.
/// </param>
/// <param name="Rate">
/// The success rate modifier for consecutive uses of the "Protect" effect.
/// </param>
[UStruct]
public readonly partial record struct FProtectData(
    [property: UProperty(PropertyFlags.BlueprintReadOnly)] bool Active,
    [property: UProperty(PropertyFlags.BlueprintReadOnly)] float Rate
);

/// <summary>
/// Represents the data associated with the "Stockpile" effect in a Pokémon battle.
/// </summary>
/// <remarks>
/// The "Stockpile" effect tracks the number of Stockpile stages as well as boosts to
/// the Pokémon's Defense and Special Defense stats. This effect is associated with moves
/// like Stockpile and Spit Up.
/// </remarks>
/// <param name="Stockpile">
/// The current Stockpile count, representing the number of stages accumulated by the Pokémon.
/// A value greater than 0 indicates active Stockpile levels.
/// </param>
/// <param name="Def">
/// The Defense stat boost associated with the current Stockpile count.
/// This value reflects the increase in physical resistance.
/// </param>
/// <param name="SpDef">
/// The Special Defense stat boost associated with the current Stockpile count.
/// This value reflects the increase in special resistance.
/// </param>
[UStruct]
public readonly partial record struct FStockpileData(
    [property: UProperty(PropertyFlags.BlueprintReadOnly)] int Stockpile,
    [property: UProperty(PropertyFlags.BlueprintReadOnly)] int Def,
    [property: UProperty(PropertyFlags.BlueprintReadOnly)] int SpDef
);

/// <summary>
/// Represents the data associated with the "Transform" effect in a Pokémon battle.
/// </summary>
/// <remarks>
/// The "Transform" effect allows a Pokémon to change its form or appearance to resemble another Pokémon.
/// This includes copying moves, stats, and possibly other attributes for the duration of the effect.
/// </remarks>
/// <param name="Transformed">
/// Indicates whether the Pokémon has successfully transformed. A value of true means the transformation has occurred.
/// </param>
/// <param name="Pokemon">
/// The handle to the species of the Pokémon that the transformation is based on. Specifies the target form.
/// </param>
[UStruct]
public readonly partial record struct FTransformEffectData(
    [property: UProperty(PropertyFlags.BlueprintReadOnly)] bool Transformed,
    [property: UProperty(PropertyFlags.BlueprintReadOnly)] FSpeciesHandle Pokemon
);

/// <summary>
/// Represents the data associated with the "Trapping" effect in a Pokémon battle.
/// </summary>
/// <remarks>
/// The "Trapping" effect immobilizes the opponent's Pokémon for a specific number of turns.
/// This effect also retains information about the move used to apply the effect and the user of the move.
/// </remarks>
/// <param name="Turns">
/// The number of turns the "Trapping" effect remains active. A value of 0 indicates that the effect is inactive.
/// </param>
/// <param name="Move">
/// The move responsible for inducing the "Trapping" effect.
/// </param>
/// <param name="User">
/// The user of the move that caused the "Trapping" effect.
/// </param>
[UStruct]
public readonly partial record struct FTrappingEffectData(
    [property: UProperty(PropertyFlags.BlueprintReadOnly)] int Turns,
    [property: UProperty(PropertyFlags.BlueprintReadOnly)] FMoveHandle Move,
    [property: UProperty(PropertyFlags.BlueprintReadOnly)] FActionTarget User
)
{
    /// <summary>
    /// Represents a default or uninitialized state for an instance of <see cref="FTrappingEffectData"/>.
    /// </summary>
    /// <remarks>
    /// This static property provides a standardized way to reference a non-active "Trapping" effect.
    /// It initializes the effect with 0 turns (inactive), no move, and no specific user.
    /// </remarks>
    public static FTrappingEffectData None => new(0, FMoveHandle.None, FActionTarget.None);
}

/// <summary>
/// Represents a collection of battle effects that can be applied to a Pokémon (battler) during a battle.
/// </summary>
/// <remarks>
/// Each effect in this class corresponds to a specific mechanic or condition within a Pokémon battle.
/// The effects are represented using static properties, allowing them to be accessed globally as
/// predefined handles. These effects may influence the battler's behavior, stats, moves, or interactions
/// with its environment and other battlers.
/// </remarks>
public static class BattlerEffects
{
    /// <summary>
    /// Represents the Aqua Ring effect, which restores a small amount of HP each turn.
    /// </summary>
    /// <remarks>
    /// Aqua Ring surrounds the Pokémon with a veil of water that restores 1/16 of its max HP at the end of each turn.
    /// </remarks>
    public static EffectHandle<FBoolEffectData> AquaRing { get; } = new(GameplayTags.Effect_Battler_AquaRing);

    /// <summary>
    /// Represents the Attract effect, which causes infatuation in the target Pokémon.
    /// </summary>
    /// <remarks>
    /// When a Pokémon becomes infatuated, it has a 50% chance of being unable to perform a move each turn.
    /// Only affects Pokémon of the opposite gender.
    /// </remarks>
    public static EffectHandle<FActionTarget> Attract { get; } =
        new(GameplayTags.Effect_Battler_Attract, FActionTarget.None);

    /// <summary>
    /// Represents the Baneful Bunker effect, which protects the Pokémon and poisons any attacker making contact.
    /// </summary>
    /// <remarks>
    /// Similar to Protect, but additionally poisons any Pokémon that makes contact with the user while the effect is active.
    /// </remarks>
    public static EffectHandle<FBoolEffectData> BanefulBunker { get; } = new(GameplayTags.Effect_Battler_BanefulBunker);

    /// <summary>
    /// Represents the Beak Blast effect, which burns any Pokémon that makes contact while charging.
    /// </summary>
    /// <remarks>
    /// The user heats up its beak in preparation for an attack. If hit by a contact move while charging,
    /// the attacker becomes burned.
    /// </remarks>
    public static EffectHandle<FBoolEffectData> BeakBlast { get; } = new(GameplayTags.Effect_Battler_BeakBlast);

    /// <summary>
    /// Represents the Bide effect, which stores damage taken and retaliates with double power.
    /// </summary>
    /// <remarks>
    /// The user endures attacks for two turns, then strikes back with double the damage it received.
    /// </remarks>
    public static EffectHandle<FBideEffectData> Bide { get; } =
        new(GameplayTags.Effect_Battler_Bide, FBideEffectData.None);

    /// <summary>
    /// Represents the Burn Up effect, which removes the Fire-type from the user.
    /// </summary>
    /// <remarks>
    /// After using Burn Up, the user loses its Fire-type. Can only be used by Fire-type Pokémon.
    /// </remarks>
    public static EffectHandle<FBoolEffectData> BurnUp { get; } = new(GameplayTags.Effect_Battler_BurnUp);

    /// <summary>
    /// Represents the Charge effect, which powers up Electric-type moves.
    /// </summary>
    /// <remarks>
    /// The user charges power for one turn, doubling the power of its next Electric-type move.
    /// </remarks>
    public static EffectHandle<FIntEffectData> Charge { get; } = new(GameplayTags.Effect_Battler_Charge);

    /// <summary>
    /// Represents the Choice Band effect, which locks the user into a single move.
    /// </summary>
    /// <remarks>
    /// When holding a Choice item, the Pokémon can only use the first move it selects until it switches out.
    /// </remarks>
    public static EffectHandle<FMoveHandle> ChoiceBand { get; } = new(GameplayTags.Effect_Battler_ChoiceBand);

    /// <summary>
    /// Represents the Confusion effect, which may cause the Pokémon to hurt itself.
    /// </summary>
    /// <remarks>
    /// A confused Pokémon has a 33% chance of hurting itself instead of executing its selected move.
    /// The effect typically lasts for 2-5 turns.
    /// </remarks>
    public static EffectHandle<FIntEffectData> Confusion { get; } = new(GameplayTags.Effect_Battler_Confusion);

    /// <summary>
    /// Represents the Counter effect, which retaliates against physical attacks.
    /// </summary>
    /// <remarks>
    /// Returns double the damage of the last physical attack received.
    /// Only works against physical moves.
    /// </remarks>
    public static EffectHandle<FCounterEffectData> Counter { get; } =
        new(GameplayTags.Effect_Battler_Counter, FCounterEffectData.None);

    /// <summary>
    /// Represents the Curse effect, which has different effects based on the user's type.
    /// </summary>
    /// <remarks>
    /// For Ghost-type Pokémon, loses 50% HP to curse the target. For non-Ghost types,
    /// lowers Speed but raises Attack and Defense.
    /// </remarks>
    public static EffectHandle<FBoolEffectData> Curse { get; } = new(GameplayTags.Effect_Battler_Curse);

    /// <summary>
    /// Represents the Dancer ability effect, which allows copying of dance moves.
    /// </summary>
    /// <remarks>
    /// When another Pokémon uses a dance move, the Pokémon with Dancer will automatically use the same move.
    /// </remarks>
    public static EffectHandle<FBoolEffectData> Dancer { get; } = new(GameplayTags.Effect_Battler_Dancer);

    /// <summary>
    /// Represents the Defense Curl effect, which raises defense and enhances rolling moves.
    /// </summary>
    /// <remarks>
    /// Raises the user's Defense by one stage and doubles the power of Rollout and Ice Ball.
    /// </remarks>
    public static EffectHandle<FBoolEffectData> DefenseCurl { get; } = new(GameplayTags.Effect_Battler_DefenseCurl);

    /// <summary>
    /// Represents the Destiny Bond effect, which faints the attacker if the user faints.
    /// </summary>
    /// <remarks>
    /// If the user faints due to an opponent's attack while this effect is active,
    /// the attacking Pokémon also faints.
    /// </remarks>
    public static EffectHandle<FDestinyBondEffectData> DestinyBond { get; } =
        new(GameplayTags.Effect_Battler_DestinyBond);

    /// <summary>
    /// Represents the Disable effect, which prevents the use of a move for several turns.
    /// </summary>
    /// <remarks>
    /// Prevents the target from using its last move for 4 turns.
    /// The effect ends if the disabled move runs out of PP.
    /// </remarks>
    public static EffectHandle<FMoveTurnEffectData> Disable { get; } = new(GameplayTags.Effect_Battler_Disable);

    /// <summary>
    /// Represents the Electrify effect, which changes the target's move to Electric-type.
    /// </summary>
    /// <remarks>
    /// Changes the type of the target's next move to Electric-type.
    /// Affects moves used in the same turn after this effect is applied.
    /// </remarks>
    public static EffectHandle<FBoolEffectData> Electrify { get; } = new(GameplayTags.Effect_Battler_Electrify);

    /// <summary>
    /// Represents the Embargo effect, which prevents item usage.
    /// </summary>
    /// <remarks>
    /// Prevents the target from using its held item for 5 turns.
    /// The target cannot use items or gain their effects while under Embargo.
    /// </remarks>
    public static EffectHandle<FIntEffectData> Embargo { get; } = new(GameplayTags.Effect_Battler_Embargo);

    /// <summary>
    /// Represents the Encore effect, which forces repeated use of a move.
    /// </summary>
    /// <remarks>
    /// Forces the target to repeat only its last used move for 3 turns.
    /// The effect ends early if the move runs out of PP.
    /// </remarks>
    public static EffectHandle<FMoveTurnEffectData> Encore { get; } = new(GameplayTags.Effect_Battler_Encore);

    /// <summary>
    /// Represents the Endure effect, which allows survival with 1 HP.
    /// </summary>
    /// <remarks>
    /// Allows the user to survive any single attack with at least 1 HP.
    /// Successive uses have a decreased chance of success.
    /// </remarks>
    public static EffectHandle<FBoolEffectData> Endure { get; } = new(GameplayTags.Effect_Battler_Endure);

    /// <summary>
    /// Represents an additional type applied to a Pokémon.
    /// </summary>
    /// <remarks>
    /// Adds an extra type to the Pokémon, affecting its type matchups and resistances.
    /// Used by effects like Forest's Curse and Trick-or-Treat.
    /// </remarks>
    public static EffectHandle<FTypeHandle> ExtraType { get; } = new(GameplayTags.Effect_Battler_ExtraType);

    /// <summary>
    /// Represents the first Pledge move used in a combination.
    /// </summary>
    /// <remarks>
    /// Tracks the first Pledge move used in a turn, enabling combination effects
    /// when followed by another Pledge move from an ally.
    /// </remarks>
    public static EffectHandle<FGameplayTag> FirstPledge { get; } = new(GameplayTags.Effect_Battler_FirstPledge);

    /// <summary>
    /// Represents the Flash Fire effect, which boosts Fire-type moves when hit by one.
    /// </summary>
    /// <remarks>
    /// When hit by a Fire-type move, the user becomes immune to Fire-type moves
    /// and receives a 50% boost to its own Fire-type moves.
    /// </remarks>
    public static EffectHandle<FBoolEffectData> FlashFire { get; } = new(GameplayTags.Effect_Battler_FlashFire);

    /// <summary>
    /// Represents the Flinch effect, which prevents the Pokémon from moving.
    /// </summary>
    /// <remarks>
    /// Causes the affected Pokémon to be unable to make a move for one turn.
    /// Only affects Pokémon that haven't moved yet in the current turn.
    /// </remarks>
    public static EffectHandle<FBoolEffectData> Flinch { get; } = new(GameplayTags.Effect_Battler_Flinch);

    /// <summary>
    /// Represents the Focus Energy effect, which increases critical hit ratio.
    /// </summary>
    /// <remarks>
    /// Raises the user's critical hit ratio by two stages.
    /// The effect lasts until the Pokémon is switched out.
    /// </remarks>
    public static EffectHandle<FBoolEffectData> FocusEnergy { get; } = new(GameplayTags.Effect_Battler_FocusEnergy);

    /// <summary>
    /// Represents the Focus Punch charging effect.
    /// </summary>
    /// <remarks>
    /// The user focuses its energy. If hit by a damaging attack while focusing,
    /// the Focus Punch will fail when executed.
    /// </remarks>
    public static EffectHandle<FBoolEffectData> FocusPunch { get; } = new(GameplayTags.Effect_Battler_FocusPunch);

    /// <summary>
    /// Represents the Follow Me effect, which redirects attacks to the user.
    /// </summary>
    /// <remarks>
    /// Draws in all single-target moves from opposing Pokémon to hit the user.
    /// Commonly used in double and triple battles for protection strategies.
    /// </remarks>
    public static EffectHandle<FIntEffectData> FollowMe { get; } = new(GameplayTags.Effect_Battler_FollowMe);

    /// <summary>
    /// Represents the Foresight effect, which enables hitting Ghost-types with Normal/Fighting moves.
    /// </summary>
    /// <remarks>
    /// Identifies Ghost-type Pokémon, allowing them to be hit by Normal and Fighting-type moves.
    /// Also negates evasiveness boosts of the target.
    /// </remarks>
    public static EffectHandle<FBoolEffectData> Foresight { get; } = new(GameplayTags.Effect_Battler_Foresight);

    /// <summary>
    /// Represents the Fury Cutter effect, which increases in power with consecutive hits.
    /// </summary>
    /// <remarks>
    /// The power of Fury Cutter doubles with each successful hit, up to a maximum of five times.
    /// The count resets if the move misses or a different move is used.
    /// </remarks>
    public static EffectHandle<FIntEffectData> FuryCutter { get; } = new(GameplayTags.Effect_Battler_FuryCutter);

    /// <summary>
    /// Represents the Gastro Acid effect, which suppresses the target's ability.
    /// </summary>
    /// <remarks>
    /// Suppresses the target's ability, preventing it from being activated.
    /// The effect lasts until the Pokémon switches out.
    /// </remarks>
    public static EffectHandle<FBoolEffectData> GastroAcid { get; } = new(GameplayTags.Effect_Battler_GastroAcid);

    /// <summary>
    /// Represents the consumed state of a type-enhancing Gem.
    /// </summary>
    /// <remarks>
    /// Tracks which type of Gem has been consumed, preventing multiple Gem boosts
    /// from being applied to the same move.
    /// </remarks>
    public static EffectHandle<FItemHandle> GemConsumed { get; } = new(GameplayTags.Effect_Battler_GemConsumed);

    /// <summary>
    /// Represents the Grudge effect, which depletes PP when the user faints.
    /// </summary>
    /// <remarks>
    /// If the user faints from a move while under this effect,
    /// that move loses all its remaining PP.
    /// </remarks>
    public static EffectHandle<FBoolEffectData> Grudge { get; } = new(GameplayTags.Effect_Battler_Grudge);

    /// <summary>
    /// Represents the Heal Block effect, which prevents healing.
    /// </summary>
    /// <remarks>
    /// Prevents the target from using healing moves and restoring HP through abilities or held items.
    /// The effect typically lasts for 5 turns.
    /// </remarks>
    public static EffectHandle<FIntEffectData> HealBlock { get; } = new(GameplayTags.Effect_Battler_HealBlock);

    /// <summary>
    /// Represents the Helping Hand effect, which boosts an ally's move power.
    /// </summary>
    /// <remarks>
    /// Increases the power of the target ally's move by 50%.
    /// Only works in double or triple battles.
    /// </remarks>
    public static EffectHandle<FBoolEffectData> HelpingHand { get; } = new(GameplayTags.Effect_Battler_HelpingHand);

    /// <summary>
    /// Represents the Hyper Beam recharge effect.
    /// </summary>
    /// <remarks>
    /// Forces the user to skip their next turn to recharge after using powerful moves
    /// like Hyper Beam or Giga Impact.
    /// </remarks>
    public static EffectHandle<FIntEffectData> HyperBeam { get; } = new(GameplayTags.Effect_Battler_HyperBeam);

    /// <summary>
    /// Represents the Illusion effect, which disguises a Pokémon as another.
    /// </summary>
    /// <remarks>
    /// Makes the Pokémon appear as the last non-fainted Pokémon in the party.
    /// The illusion breaks when the Pokémon is hit by a damaging move.
    /// </remarks>
    public static EffectHandle<FIllusionEffectData> Illusion { get; } = new(GameplayTags.Effect_Battler_Illusion);

    /// <summary>
    /// Represents the Imprison effect, which prevents opponents from using shared moves.
    /// </summary>
    /// <remarks>
    /// Prevents opposing Pokémon from using any moves that the user also knows.
    /// The effect lasts until the user switches out.
    /// </remarks>
    public static EffectHandle<FBoolEffectData> Imprison { get; } = new(GameplayTags.Effect_Battler_Imprison);

    /// <summary>
    /// Represents the Ingrain effect, which roots the Pokémon and restores HP.
    /// </summary>
    /// <remarks>
    /// Plants roots that restore 1/16 of max HP each turn.
    /// Prevents the user from being forced to switch out.
    /// </remarks>
    public static EffectHandle<FBoolEffectData> Ingrain { get; } = new(GameplayTags.Effect_Battler_Ingrain);

    /// <summary>
    /// Represents the Instruct effect, which makes a target repeat its last move.
    /// </summary>
    /// <remarks>
    /// Forces the target to immediately use its most recently used move again.
    /// Cannot be used on moves that are charging or recharging.
    /// </remarks>
    public static EffectHandle<FBoolEffectData> Instruct { get; } = new(GameplayTags.Effect_Battler_Instruct);

    /// <summary>
    /// Represents the state of being instructed to repeat a move.
    /// </summary>
    /// <remarks>
    /// Indicates that a Pokémon has been affected by the Instruct move and
    /// must repeat its last used move.
    /// </remarks>
    public static EffectHandle<FBoolEffectData> Instructed { get; } = new(GameplayTags.Effect_Battler_Instructed);

    /// <summary>
    /// Represents the Jaw Lock effect, which prevents switching out.
    /// </summary>
    /// <remarks>
    /// Prevents both the user and the target from switching out or fleeing.
    /// The effect ends when either Pokémon faints or is removed from battle.
    /// </remarks>
    public static EffectHandle<FActionTarget> JawLock { get; } =
        new(GameplayTags.Effect_Battler_JawLock, FActionTarget.None);

    /// <summary>
    /// Represents the King's Shield protective effect.
    /// </summary>
    /// <remarks>
    /// Protects the user from damage and lowers the Attack stat of
    /// attackers that make contact. Success rate decreases with consecutive uses.
    /// </remarks>
    public static EffectHandle<FBoolEffectData> KingsShield { get; } = new(GameplayTags.Effect_Battler_KingsShield);

    /// <summary>
    /// Represents the Laser Focus effect, which ensures the next hit is critical.
    /// </summary>
    /// <remarks>
    /// Guarantees that the user's next move will result in a critical hit.
    /// The effect lasts until the end of the next turn.
    /// </remarks>
    public static EffectHandle<FIntEffectData> LaserFocus { get; } = new(GameplayTags.Effect_Battler_LaserFocus);

    /// <summary>
    /// Represents the Leech Seed effect, which drains HP each turn.
    /// </summary>
    /// <remarks>
    /// Plants a seed on the target that drains 1/8 of its max HP each turn
    /// and restores it to the user. Doesn't affect Grass-type Pokémon.
    /// </remarks>
    public static EffectHandle<FActionTarget> LeechSeed { get; } =
        new(GameplayTags.Effect_Battler_LeechSeed, FActionTarget.None);

    /// <summary>
    /// Represents the Lock-On effect, which ensures the next move hits.
    /// </summary>
    /// <remarks>
    /// Ensures that the user's next move will hit the target regardless of
    /// accuracy or evasiveness. Effect lasts for two turns.
    /// </remarks>
    public static EffectHandle<FTargetTurnEffectData> LockOn { get; } =
        new(GameplayTags.Effect_Battler_LockOn, FTargetTurnEffectData.None);

    /// <summary>
    /// Represents the Magic Bounce ability effect.
    /// </summary>
    /// <remarks>
    /// Reflects status moves and certain non-damaging moves back at the user.
    /// Does not reflect moves used by allies.
    /// </remarks>
    public static EffectHandle<FBoolEffectData> MagicBounce { get; } = new(GameplayTags.Effect_Battler_MagicBounce);

    /// <summary>
    /// Represents the Magic Coat effect, which reflects status moves.
    /// </summary>
    /// <remarks>
    /// Reflects status moves back at the user for one turn.
    /// Similar to Magic Bounce but only lasts for one turn.
    /// </remarks>
    public static EffectHandle<FBoolEffectData> MagicCoat { get; } = new(GameplayTags.Effect_Battler_MagicCoat);

    /// <summary>
    /// Represents the Magnet Rise effect, which makes the user float.
    /// </summary>
    /// <remarks>
    /// The user levitates and becomes immune to Ground-type moves for five turns.
    /// The effect ends if the user is grounded by another effect.
    /// </remarks>
    public static EffectHandle<FBoolEffectData> MagnetRise { get; } = new(GameplayTags.Effect_Battler_MagnetRise);

    /// <summary>
    /// Represents the Mean Look effect, which prevents escape.
    /// </summary>
    /// <remarks>
    /// Prevents the target from fleeing or switching out.
    /// The effect ends if the target is removed from battle.
    /// </remarks>
    public static EffectHandle<FActionTarget> MeanLook { get; } =
        new(GameplayTags.Effect_Battler_MeanLook, FActionTarget.None);

    /// <summary>
    /// Represents the Me First effect, which copies and powers up the target's move.
    /// </summary>
    /// <remarks>
    /// Allows the user to use the target's intended move with a 50% power boost.
    /// Only works on damaging moves.
    /// </remarks>
    public static EffectHandle<FBoolEffectData> MeFirst { get; } = new(GameplayTags.Effect_Battler_MeFirst);

    /// <summary>
    /// Represents the Metronome effect, which boosts consecutive move usage.
    /// </summary>
    /// <remarks>
    /// Increases the power of a move when used consecutively.
    /// Power increases by 20% with each successful use, up to 100%.
    /// </remarks>
    public static EffectHandle<FIntEffectData> Metronome { get; } = new(GameplayTags.Effect_Battler_Metronome);

    /// <summary>
    /// Represents the Micle Berry effect, which boosts accuracy.
    /// </summary>
    /// <remarks>
    /// Increases the accuracy of the next move by 20% when consumed.
    /// Typically activated when HP falls below 25%.
    /// </remarks>
    public static EffectHandle<FBoolEffectData> MicleBerry { get; } = new(GameplayTags.Effect_Battler_MicleBerry);

    /// <summary>
    /// Represents the Minimize effect, which sharply raises evasiveness.
    /// </summary>
    /// <remarks>
    /// Sharply raises evasiveness by two stages.
    /// Makes the user more vulnerable to certain moves like Stomp.
    /// </remarks>
    public static EffectHandle<FBoolEffectData> Minimize { get; } = new(GameplayTags.Effect_Battler_Minimize);

    /// <summary>
    /// Represents the Miracle Eye effect, which enables hitting Dark-types with Psychic moves.
    /// </summary>
    /// <remarks>
    /// Enables Psychic-type moves to hit Dark-type Pokémon.
    /// Also negates the target's evasiveness boosts.
    /// </remarks>
    public static EffectHandle<FBoolEffectData> MiracleEye { get; } = new(GameplayTags.Effect_Battler_MiracleEye);

    /// <summary>
    /// Represents the Mirror Coat effect, which counters special attacks.
    /// </summary>
    /// <remarks>
    /// Returns double the damage of a special attack back to the attacker.
    /// Only works against special moves.
    /// </remarks>
    public static EffectHandle<FCounterEffectData> MirrorCoat { get; } =
        new(GameplayTags.Effect_Battler_MirrorCoat, FCounterEffectData.None);

    /// <summary>
    /// Represents the effect of moving next in the turn order.
    /// </summary>
    /// <remarks>
    /// Indicates that the Pokémon will move next in the current turn,
    /// typically due to moves or abilities that affect turn order.
    /// </remarks>
    public static EffectHandle<FBoolEffectData> MoveNext { get; } = new(GameplayTags.Effect_Battler_MoveNext);

    /// <summary>
    /// Represents the Mud Sport effect, which weakens Electric moves.
    /// </summary>
    /// <remarks>
    /// Weakens the power of Electric-type moves by 50%.
    /// Affects all Pokémon on the field.
    /// </remarks>
    public static EffectHandle<FBoolEffectData> MudSport { get; } = new(GameplayTags.Effect_Battler_MudSport);

    /// <summary>
    /// Represents the Nightmare effect, which damages sleeping targets.
    /// </summary>
    /// <remarks>
    /// A sleeping Pokémon loses 1/4 of its max HP each turn.
    /// The effect ends when the target wakes up.
    /// </remarks>
    public static EffectHandle<FBoolEffectData> Nightmare { get; } = new(GameplayTags.Effect_Battler_Nightmare);

    /// <summary>
    /// Represents the No Retreat effect, which boosts stats but prevents switching.
    /// </summary>
    /// <remarks>
    /// Raises all stats by one stage but prevents the user from switching out.
    /// The effect remains until the Pokémon faints or is forced out.
    /// </remarks>
    public static EffectHandle<FBoolEffectData> NoRetreat { get; } = new(GameplayTags.Effect_Battler_NoRetreat);

    /// <summary>
    /// Represents the Obstruct effect, which protects and lowers Defense.
    /// </summary>
    /// <remarks>
    /// Protects the user and sharply lowers the Defense of attackers making contact.
    /// Success rate decreases with consecutive uses.
    /// </remarks>
    public static EffectHandle<FBoolEffectData> Obstruct { get; } = new(GameplayTags.Effect_Battler_Obstruct);

    /// <summary>
    /// Represents the Octolock effect, which traps and lowers defenses.
    /// </summary>
    /// <remarks>
    /// Prevents the target from fleeing and lowers its Defense and Special Defense
    /// each turn. The effect ends if the target is removed from battle.
    /// </remarks>
    public static EffectHandle<FActionTarget> Octolock { get; } =
        new(GameplayTags.Effect_Battler_Octolock, FActionTarget.None);

    /// <summary>
    /// Represents the Outrage effect, which forces repeated attacks.
    /// </summary>
    /// <remarks>
    /// Forces the user to attack for 2-3 turns, then becomes confused.
    /// Cannot select a different move while active.
    /// </remarks>
    public static EffectHandle<FIntEffectData> Outrage { get; } = new(GameplayTags.Effect_Battler_Outrage);

    /// <summary>
    /// Represents the Parental Bond effect, which enables hitting twice.
    /// </summary>
    /// <remarks>
    /// Allows the Pokémon to hit twice with a single move.
    /// The second hit deals reduced damage.
    /// </remarks>
    public static EffectHandle<FIntEffectData> ParentalBond { get; } = new(GameplayTags.Effect_Battler_ParentalBond);

    /// <summary>
    /// Represents the Perish Song effect, which counts down to fainting.
    /// </summary>
    /// <remarks>
    /// Any Pokémon that hears Perish Song faints in three turns unless switched out.
    /// The counter decreases by one each turn.
    /// </remarks>
    public static EffectHandle<FTargetTurnEffectData> PerishSong { get; } =
        new(GameplayTags.Effect_Battler_PerishSong, FTargetTurnEffectData.None);

    /// <summary>
    /// Represents the Pickup effect, which finds items after battles.
    /// </summary>
    /// <remarks>
    /// Allows the Pokémon to pick up items after battle.
    /// The items found can vary based on the level of the Pokémon.
    /// </remarks>
    public static EffectHandle<FPickupEffectData> Pickup { get; } = new(GameplayTags.Effect_Battler_Pickup);

    /// <summary>
    /// Represents the Pinch effect, which activates at low HP.
    /// </summary>
    /// <remarks>
    /// Indicates that the Pokémon is at low HP (usually 1/3 or less),
    /// which can trigger certain abilities or effects.
    /// </remarks>
    public static EffectHandle<FBoolEffectData> Pinch { get; } = new(GameplayTags.Effect_Battler_Pinch);

    /// <summary>
    /// Represents the Powder effect, which damages Fire-move users.
    /// </summary>
    /// <remarks>
    /// If the affected Pokémon uses a Fire-type move, it takes damage
    /// instead of executing the move.
    /// </remarks>
    public static EffectHandle<FBoolEffectData> Powder { get; } = new(GameplayTags.Effect_Battler_Powder);

    /// <summary>
    /// Represents the Power Trick effect, which swaps Attack and Defense.
    /// </summary>
    /// <remarks>
    /// Switches the user's Attack and Defense stats.
    /// The effect lasts until the user leaves the battle.
    /// </remarks>
    public static EffectHandle<FBoolEffectData> PowerTrick { get; } = new(GameplayTags.Effect_Battler_PowerTrick);

    /// <summary>
    /// Represents the Prankster effect, which prioritizes status moves.
    /// </summary>
    /// <remarks>
    /// Increases the priority of status moves by one stage.
    /// Does not affect Dark-type Pokémon.
    /// </remarks>
    public static EffectHandle<FBoolEffectData> Prankster { get; } = new(GameplayTags.Effect_Battler_Prankster);

    /// <summary>
    /// Represents the priority boost from abilities.
    /// </summary>
    /// <remarks>
    /// Indicates that the Pokémon has received a priority boost from an ability,
    /// affecting its move order in battle.
    /// </remarks>
    public static EffectHandle<FBoolEffectData> PriorityAbility { get; } =
        new(GameplayTags.Effect_Battler_PriorityAbility);

    /// <summary>
    /// Represents the priority boost from held items.
    /// </summary>
    /// <remarks>
    /// Indicates that the Pokémon has received a priority boost from a held item,
    /// affecting its move order in battle.
    /// </remarks>
    public static EffectHandle<FBoolEffectData> PriorityItem { get; } = new(GameplayTags.Effect_Battler_PriorityItem);

    /// <summary>
    /// Represents the Protect effect, which prevents damage and effects.
    /// </summary>
    /// <remarks>
    /// Protects the user from most moves for one turn.
    /// Success rate decreases with consecutive uses.
    /// </remarks>
    public static EffectHandle<FProtectData> Protect { get; } =
        new(GameplayTags.Effect_Battler_Protect, new FProtectData(false, 1.0f));

    /// <summary>
    /// Represents the Quash effect, which delays a target's move.
    /// </summary>
    /// <remarks>
    /// Forces the target to move last in the turn.
    /// Only works in double or triple battles.
    /// </remarks>
    public static EffectHandle<FIntEffectData> Quash { get; } = new(GameplayTags.Effect_Battler_Quash);

    /// <summary>
    /// Represents the Rage effect, which boosts Attack when hit.
    /// </summary>
    /// <remarks>
    /// Raises the user's Attack by one stage each time it takes damage.
    /// Continues until the user uses a different move.
    /// </remarks>
    public static EffectHandle<FBoolEffectData> Rage { get; } = new(GameplayTags.Effect_Battler_Rage);

    /// <summary>
    /// Represents the Rage Powder effect, which redirects attacks.
    /// </summary>
    /// <remarks>
    /// Redirects all opponents' moves to the user for one turn.
    /// Similar to Follow Me but doesn't affect Grass-type Pokémon.
    /// </remarks>
    public static EffectHandle<FBoolEffectData> RagePowder { get; } = new(GameplayTags.Effect_Battler_RagePowder);

    /// <summary>
    /// Represents the Rollout effect, which increases power each turn.
    /// </summary>
    /// <remarks>
    /// Continues for five turns, doubling in power each turn.
    /// Power resets if the move fails or is interrupted.
    /// </remarks>
    public static EffectHandle<FIntEffectData> Rollout { get; } = new(GameplayTags.Effect_Battler_Rollout);

    /// <summary>
    /// Represents the Roost effect, which removes Flying-type temporarily.
    /// </summary>
    /// <remarks>
    /// Heals the user but removes its Flying-type until the end of the turn.
    /// Flying-type moves can't be used while grounded.
    /// </remarks>
    public static EffectHandle<FBoolEffectData> Roost { get; } = new(GameplayTags.Effect_Battler_Roost);

    /// <summary>
    /// Represents the Shell Trap effect, which counters physical moves.
    /// </summary>
    /// <remarks>
    /// Sets a trap that damages attackers using physical moves.
    /// Only activates if the user is hit by a physical move that turn.
    /// </remarks>
    public static EffectHandle<FBoolEffectData> ShellTrap { get; } = new(GameplayTags.Effect_Battler_ShellTrap);

    /// <summary>
    /// Represents the Sky Drop effect, which carries targets into the sky.
    /// </summary>
    /// <remarks>
    /// Takes the target into the sky for one turn, making both Pokémon semi-invulnerable.
    /// Drops the target on the next turn for damage.
    /// </remarks>
    public static EffectHandle<FActionTarget> SkyDrop { get; } =
        new(GameplayTags.Effect_Battler_SkyDrop, FActionTarget.None);

    /// <summary>
    /// Represents the Slow Start effect, which halves Attack and Speed.
    /// </summary>
    /// <remarks>
    /// Halves the Pokémon's Attack and Speed stats for five turns.
    /// Typically associated with the Slow Start ability.
    /// </remarks>
    public static EffectHandle<FIntEffectData> SlowStart { get; } = new(GameplayTags.Effect_Battler_SlowStart);

    /// <summary>
    /// Represents the Smack Down effect, which grounds Flying Pokémon.
    /// </summary>
    /// <remarks>
    /// Removes the target's Ground-type immunity and makes it susceptible to Ground moves.
    /// The effect persists until the Pokémon switches out.
    /// </remarks>
    public static EffectHandle<FBoolEffectData> SmackDown { get; } = new(GameplayTags.Effect_Battler_SmackDown);

    /// <summary>
    /// Represents the effect of the move "Snatch" on a battler within a Pokémon battle.
    /// </summary>
    /// <remarks>
    /// This property manages the "Snatch" effect applied to a Pokémon. "Snatch" allows the user
    /// to steal certain beneficial effects that the target Pokémon attempts to apply to itself. The data is represented
    /// as an integer, which could correspond to turns remaining or other effect-specific metadata.
    /// </remarks>
    public static EffectHandle<FIntEffectData> Snatch { get; } = new(GameplayTags.Effect_Battler_Snatch);

    /// <summary>
    /// Represents the effect data associated with the "Spiky Shield" status for a battler in combat.
    /// </summary>
    /// <remarks>
    /// This property, when active, applies the "Spiky Shield" effect to a combatant. It prevents damage from direct attacks
    /// while potentially inflicting recoil damage to attackers. The effect is initialized using the <see cref="FGameplayTag"/>
    /// and is tied to the battler's status in a battle context.
    /// </remarks>
    public static EffectHandle<FBoolEffectData> SpikyShield { get; } = new(GameplayTags.Effect_Battler_SpikyShield);

    /// <summary>
    /// Represents the Spotlight effect applied to a battler in battle.
    /// </summary>
    /// <remarks>
    /// This effect is used to single out a battler, temporarily making it the target of moves or actions that
    /// can be affected by such a designation. The underlying effect data utilizes an integer value to manage its state or duration.
    /// </remarks>
    public static EffectHandle<FIntEffectData> Spotlight { get; } = new(GameplayTags.Effect_Battler_Spotlight);

    /// <summary>
    /// Represents an effect related to the "Stockpile" mechanic for a battler in combat.
    /// </summary>
    /// <remarks>
    /// This static property links to the "Stockpile" effect, which tracks data such as the current stockpile count,
    /// alongside temporary defense and special defense values accumulated during battle.
    /// It uses the <see cref="FStockpileData"/> structure to encapsulate this information.
    /// </remarks>
    public static EffectHandle<FStockpileData> Stockpile { get; } = new(GameplayTags.Effect_Battler_Stockpile);

    /// <summary>
    /// Represents the effect of the move "Substitute," allowing a battler to mitigate damage by creating a decoy.
    /// </summary>
    /// <remarks>
    /// This property handles the configuration and tracking of the "Substitute" effect within the battle system.
    /// It uses an integer value to specify the remaining health of the substitute, enabling the logic that decides
    /// when the substitute is destroyed or remains active. The effect is associated with the gameplay tag "Effect.Battler.Substitute."
    /// </remarks>
    public static EffectHandle<FIntEffectData> Substitute { get; } = new(GameplayTags.Effect_Battler_Substitute);

    /// <summary>
    /// Represents the effect related to the "Tar Shot" move in battle, applied to a specific battler.
    /// </summary>
    /// <remarks>
    /// This property manages the state and data for the "Tar Shot" effect, which can interact with other
    /// mechanics within the battle system. It utilizes a boolean-based effect data structure to track
    /// its activation state.
    /// </remarks>
    public static EffectHandle<FBoolEffectData> TarShot { get; } = new(GameplayTags.Effect_Battler_TarShot);

    /// <summary>
    /// Represents the Taunt effect applied to a battler in a Pokémon battle.
    /// </summary>
    /// <remarks>
    /// The Taunt effect restricts the target from using non-damaging moves while the effect is active.
    /// This property provides access to the underlying data, including the duration of the effect,
    /// represented by an integer value.
    /// </remarks>
    public static EffectHandle<FIntEffectData> Taunt { get; } = new(GameplayTags.Effect_Battler_Taunt);

    /// <summary>
    /// Represents the effect responsible for handling the Telekinesis status in battle.
    /// </summary>
    /// <remarks>
    /// This property links to the specific gameplay tag for the Telekinesis effect, where the effect data is encapsulated
    /// within an integer-based structure (<see cref="FIntEffectData"/>). The Telekinesis status typically causes the affected
    /// target to levitate for a limited duration, making it temporarily immune to Ground-type moves and hazards like Spikes.
    /// </remarks>
    public static EffectHandle<FIntEffectData> Telekinesis { get; } = new(GameplayTags.Effect_Battler_Telekinesis);

    /// <summary>
    /// Represents the "Throat Chop" effect applied to a battler.
    /// </summary>
    /// <remarks>
    /// This effect prevents the affected battler from using sound-based moves for a certain period of time.
    /// The associated <see cref="FIntEffectData"/> tracks the duration or remaining turns of the effect, measured as an integer value.
    /// The effect is identified using the gameplay tag <c>Effect_Battler_ThroatChop</c>.
    /// </remarks>
    public static EffectHandle<FIntEffectData> ThroatChop { get; } = new(GameplayTags.Effect_Battler_ThroatChop);

    /// <summary>
    /// Represents the "Torment" effect applied to a battler, preventing the repeated use of the same move consecutively.
    /// </summary>
    /// <remarks>
    /// This property is associated with the gameplay effect "Effect.Battler.Torment" and utilizes <see cref="FBoolEffectData"/>
    /// to determine its active state. It provides game rules to enforce limitations on move usage while the effect is active.
    /// </remarks>
    public static EffectHandle<FBoolEffectData> Torment { get; } = new(GameplayTags.Effect_Battler_Torment);

    /// <summary>
    /// Represents the `Toxic` effect applied to a battler in battle, which tracks and manages the effect's progressive impact over turns.
    /// </summary>
    /// <remarks>
    /// This property defines the `Toxic` effect as an instance of <see cref="EffectHandle{FIntEffectData}"/>, using the gameplay tag
    /// "Effect.Battler.Toxic". The effect utilizes an integer value to determine and increment damage progression as the battle progresses.
    /// It is typically associated with increasing damage over time and is integral to gameplay mechanics involving status effects.
    /// </remarks>
    public static EffectHandle<FIntEffectData> Toxic { get; } = new(GameplayTags.Effect_Battler_Toxic);

    /// <summary>
    /// Represents the effect data associated with the "Transform" move, applied to a battler during a battle.
    /// </summary>
    /// <remarks>
    /// This property serves as a handle to manage the state of the "Transform" effect, which alters a battler's form to mimic another Pokémon.
    /// The effect can modify key attributes such as species data and associated behavior. The default value contains the parameters
    /// required for tracking whether the transformation has occurred and the species information of the target Pokémon.
    /// </remarks>
    public static EffectHandle<FTransformEffectData> Transform { get; } = new(GameplayTags.Effect_Battler_Transform);

    /// <summary>
    /// Represents a trapping condition effect applied to a battler during a battle.
    /// </summary>
    /// <remarks>
    /// This static property defines an effect that restricts a battler's ability to switch out or escape
    /// for a certain number of turns. The effect is linked to a specific move and is attributed to a particular
    /// user, providing detailed information about the trapping condition and its source.
    /// </remarks>
    public static EffectHandle<FTrappingEffectData> Trapping { get; } =
        new(GameplayTags.Effect_Battler_Trapping, FTrappingEffectData.None);

    /// <summary>
    /// Represents the "Truant" effect applied to a battler, indicating a state where the battler skips every other turn.
    /// </summary>
    /// <remarks>
    /// This property provides an <see cref="EffectHandle{FBoolEffectData}"/> for managing the Truant condition in battle scenarios.
    /// Battlers affected by this condition will effectively alternate between being active and inactive, based on the associated effect logic.
    /// </remarks>
    public static EffectHandle<FBoolEffectData> Truant { get; } = new(GameplayTags.Effect_Battler_Truant);

    /// <summary>
    /// Represents the effect data associated with a two-turn attack in the battle system.
    /// </summary>
    /// <remarks>
    /// This property tracks and manages the state of moves requiring multiple turns to execute fully, such as "SolarBeam" or "Dig."
    /// It references an effect that temporarily holds information about the move being prepared or executed during its duration.
    /// </remarks>
    public static EffectHandle<FMoveHandle> TwoTurnAttack { get; } = new(GameplayTags.Effect_Battler_TwoTurnAttack);

    /// <summary>
    /// Represents the "Unburden" effect associated with a battler in gameplay.
    /// </summary>
    /// <remarks>
    /// This static property is used to handle the state of the Unburden effect for a battler.
    /// It utilizes an <see cref="EffectHandle{T}"/> with a <see cref="FBoolEffectData"/> type,
    /// indicating whether the effect is active or inactive.
    /// </remarks>
    public static EffectHandle<FBoolEffectData> Unburden { get; } = new(GameplayTags.Effect_Battler_Unburden);

    /// <summary>
    /// Represents the "Uproar" status effect for a battler.
    /// </summary>
    /// <remarks>
    /// This property provides access to the "Uproar" effect, which uses an integer value to represent
    /// its state or duration. The effect is typically associated with moves or abilities that cause an uproar,
    /// preventing certain actions such as sleep during its active duration.
    /// </remarks>
    public static EffectHandle<FIntEffectData> Uproar { get; } = new(GameplayTags.Effect_Battler_Uproar);

    /// <summary>
    /// Represents the effect of Water Sport on a battler in a Pokémon battle.
    /// </summary>
    /// <remarks>
    /// This property tracks the presence of the Water Sport effect, which reduces the power of Fire-type moves.
    /// It utilizes <see cref="FBoolEffectData"/> to indicate whether the effect is currently active for the battler.
    /// </remarks>
    public static EffectHandle<FBoolEffectData> WaterSport { get; } = new(GameplayTags.Effect_Battler_WaterSport);

    /// <summary>
    /// Represents an effect that modifies the weight of a battler.
    /// </summary>
    /// <remarks>
    /// This property is part of the battler effects system and is used to handle weight changes during battles.
    /// The effect is associated with the tag <see cref="GameplayTags.Effect_Battler_WeightChange"/> and utilizes the
    /// <see cref="FIntEffectData"/> structure to store the weight modification value.
    /// </remarks>
    public static EffectHandle<FIntEffectData> WeightChange { get; } = new(GameplayTags.Effect_Battler_WeightChange);

    /// <summary>
    /// Represents the effect of "Yawn" on a battler in a Pokémon battle.
    /// </summary>
    /// <remarks>
    /// This effect indicates that the battler is subject to the "Yawn" status condition, which typically causes the
    /// battler to fall asleep at the end of the next turn unless it is prevented or cured.
    /// It utilizes an integer effect data type to track relevant parameters, such as the remaining turns before the
    /// sleep condition activates.
    /// </remarks>
    public static EffectHandle<FIntEffectData> Yawn { get; } = new(GameplayTags.Effect_Battler_Yawn);
}
