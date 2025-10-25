using UnrealSharp.Attributes;

namespace Pokemon.Battle.Effects;

/// <summary>
/// Represents the data associated with the "Echoed Voice" effect in a battle context.
/// </summary>
/// <param name="Counter">The counter value for the effect.</param>
/// <param name="Used">Indicates whether the move has been used in the current turn.</param>
/// <remarks>
/// This effect tracks progress for the move "Echoed Voice," typically counting how
/// many consecutive turns the move has been used and whether it has been activated
/// in the current turn.
/// </remarks>
[UStruct]
public readonly partial record struct FEchoedVoiceEffectData(
    [UProperty(PropertyFlags.BlueprintReadOnly)] int Counter,
    [UProperty(PropertyFlags.BlueprintReadOnly)] bool Used
);

/// <summary>
/// Provides a collection of predefined side effect handles used in battle contexts.
/// </summary>
/// <remarks>
/// This static class acts as a registry for various effects that affect a specific side of battle.
/// </remarks>
public static class SideEffects
{
    /// <summary>
    /// Represents the Aurora Veil effect, which reduces both physical and special damage.
    /// </summary>
    /// <remarks>
    /// Halves the damage from both physical and special attacks for 5 turns.
    /// Can only be used during Hail or Snow weather conditions.
    /// Duration is extended to 8 turns if the user holds Light Clay.
    /// </remarks>
    public static EffectHandle<FIntEffectData> AuroraVeil { get; } = new(GameplayTags.Effect_Side_AuroraVeil);

    /// <summary>
    /// Represents the Crafty Shield effect, which protects against status moves.
    /// </summary>
    /// <remarks>
    /// Protects the entire team from status and lowering stat moves for one turn.
    /// Does not protect against damaging moves.
    /// </remarks>
    public static EffectHandle<FBoolEffectData> CraftyShield { get; } = new(GameplayTags.Effect_Side_CraftyShield);

    /// <summary>
    /// Represents the Echoed Voice effect, which increases in power with consecutive uses.
    /// </summary>
    /// <remarks>
    /// Tracks the power increase of Echoed Voice when used on consecutive turns.
    /// Power increases by 40 each turn, up to a maximum of 200 base power.
    /// </remarks>
    public static EffectHandle<FEchoedVoiceEffectData> EchoedVoice { get; } = new(GameplayTags.Effect_Side_EchoedVoice);

    /// <summary>
    /// Represents the turn count since a Pokémon last fainted on this side.
    /// </summary>
    /// <remarks>
    /// Tracks the number of turns since a Pokémon on this side has fainted.
    /// Used for various battle mechanics and move effects.
    /// A value of -1 indicates no Pokémon has fainted yet.
    /// </remarks>
    public static EffectHandle<FIntEffectData> LastRoundFainted { get; } =
        new(GameplayTags.Effect_Side_LastRoundFainted, -1);

    /// <summary>
    /// Represents the Light Screen effect, which reduces special damage.
    /// </summary>
    /// <remarks>
    /// Reduces damage from special attacks by 50% for 5 turns.
    /// Duration is extended to 8 turns if the user holds Light Clay.
    /// In double battles, the damage reduction is 33%.
    /// </remarks>
    public static EffectHandle<FIntEffectData> LightScreen { get; } = new(GameplayTags.Effect_Side_LightScreen);

    /// <summary>
    /// Represents the Lucky Chant effect, which prevents critical hits.
    /// </summary>
    /// <remarks>
    /// Prevents critical hits against the user's team for 5 turns.
    /// Affects all Pokémon on the user's side of the field.
    /// </remarks>
    public static EffectHandle<FIntEffectData> LuckyChant { get; } = new(GameplayTags.Effect_Side_LuckyChant);

    /// <summary>
    /// Represents the Mat Block effect, which protects against damaging moves.
    /// </summary>
    /// <remarks>
    /// Protects the team from all damaging moves for one turn.
    /// Can only be used on the first turn after the user enters battle.
    /// </remarks>
    public static EffectHandle<FBoolEffectData> MatBlock { get; } = new(GameplayTags.Effect_Side_MatBlock);

    /// <summary>
    /// Represents the Mist effect, which prevents stat reduction.
    /// </summary>
    /// <remarks>
    /// Prevents stat reductions caused by opposing Pokémon's moves for 5 turns.
    /// Does not prevent self-induced stat reductions or ability effects.
    /// </remarks>
    public static EffectHandle<FIntEffectData> Mist { get; } = new(GameplayTags.Effect_Side_Mist);

    /// <summary>
    /// Represents the Quick Guard effect, which protects against priority moves.
    /// </summary>
    /// <remarks>
    /// Protects the team from moves with increased priority for one turn.
    /// Does not protect against regular or negative priority moves.
    /// </remarks>
    public static EffectHandle<FBoolEffectData> QuickGuard { get; } = new(GameplayTags.Effect_Side_QuickGuard);

    /// <summary>
    /// Represents the Rainbow effect, which enhances the power of certain moves and abilities.
    /// </summary>
    /// <remarks>
    /// Increases the effectiveness of moves that are specifically influenced by the presence of a rainbow.
    /// Typically activated by specific weather conditions, abilities, or moves.
    /// Duration and impact may vary based on game rules and other active effects.
    /// </remarks>
    public static EffectHandle<FIntEffectData> Rainbow { get; } = new(GameplayTags.Effect_Side_Rainbow);

    /// <summary>
    /// Represents the Reflect effect, which reduces incoming physical damage.
    /// </summary>
    /// <remarks>
    /// Halves the damage from physical attacks for 5 turns when active on the user's side.
    /// Duration is extended to 8 turns if the user holds Light Clay.
    /// Does not reduce damage from special attacks.
    /// </remarks>
    public static EffectHandle<FIntEffectData> Reflect { get; } = new(GameplayTags.Effect_Side_Reflect);

    /// <summary>
    /// Represents the Round effect, which facilitates the execution of the move Round in multi-battle scenarios.
    /// </summary>
    /// <remarks>
    /// Allows certain Pokémon to perform the move Round in succession during the same turn.
    /// Synchronizes with allies' use of Round, amplifying its effects and power.
    /// This effect persists only for the duration of the current turn.
    /// </remarks>
    public static EffectHandle<FBoolEffectData> Round { get; } = new(GameplayTags.Effect_Side_Round);

    /// <summary>
    /// Represents the Safeguard effect, which protects the user's side from status conditions.
    /// </summary>
    /// <remarks>
    /// Prevents all Pokémon on the user's side from being afflicted by major status conditions
    /// such as Burn, Paralysis, Sleep, Freeze, and Poison for 5 turns.
    /// Does not protect against status conditions that are self-inflicted or caused by abilities.
    /// If the user holds Light Clay, the duration is extended to 8 turns.
    /// </remarks>
    public static EffectHandle<FIntEffectData> Safeguard { get; } = new(GameplayTags.Effect_Side_Safeguard);

    /// <summary>
    /// Represents the Sea of Fire effect, which deals damage to grounded Pokémon at the end of each turn.
    /// </summary>
    /// <remarks>
    /// Causes damage to all grounded Pokémon except Fire-types at the end of every turn.
    /// The effect lasts for 5 turns once activated.
    /// The damage inflicted is based on a portion of the Pokémon's maximum HP.
    /// This effect is triggered when certain moves, such as Grass Pledge in combination with Fire Pledge, are used.
    /// </remarks>
    public static EffectHandle<FIntEffectData> SeaOfFire { get; } = new(GameplayTags.Effect_Side_SeaOfFire);

    /// <summary>
    /// Represents the Spikes effect, which damages opposing Pokémon that switch into battle.
    /// </summary>
    /// <remarks>
    /// The damage is determined by the number of layers of Spikes set on the opponent's side of the battlefield:
    /// - One layer reduces HP by 1/8.
    /// - Two layers reduce HP by 1/6.
    /// - Three layers reduce HP by 1/4.
    /// This effect does not damage Flying-type Pokémon or Pokémon with the Levitate ability unless affected by moves or items that nullify immunity to Ground-type hazards.
    /// </remarks>
    public static EffectHandle<FIntEffectData> Spikes { get; } = new(GameplayTags.Effect_Side_Spikes);

    /// <summary>
    /// Represents the Stealth Rock effect, which sets entry hazards on the opposing side of the field.
    /// </summary>
    /// <remarks>
    /// Damages opposing Pokémon that switch into battle. The damage is based on their type effectiveness
    /// against the Rock type, with Pokémon that are weak to Rock taking more damage.
    /// The effect remains until it is removed, such as by the use of Rapid Spin or Defog.
    /// </remarks>
    public static EffectHandle<FBoolEffectData> StealthRock { get; } = new(GameplayTags.Effect_Side_StealthRock);

    /// <summary>
    /// Represents the Sticky Web effect, which hinders the movement of opposing Pokémon upon entry.
    /// </summary>
    /// <remarks>
    /// Reduces the Speed of opposing Pokémon that switch into battle by one stage.
    /// This effect applies once per Pokémon, and it does not affect Flying-type Pokémon or Pokémon with Levitate unless they are grounded.
    /// It can be cleared by field-clearing moves such as Defog or Rapid Spin.
    /// </remarks>
    public static EffectHandle<FBoolEffectData> StickyWeb { get; } = new(GameplayTags.Effect_Side_StickyWeb);

    /// <summary>
    /// Represents the Swamp effect, which reduces the Speed of all Pokémon on the affected side of the field.
    /// </summary>
    /// <remarks>
    /// Lowers the Speed multiplier of all Pokémon on the affected side for the duration of the effect.
    /// The Speed reduction applies during calculations in battles where the effect is active.
    /// Typically used to hinder opponents' turn order advantage.
    /// </remarks>
    public static EffectHandle<FIntEffectData> Swamp { get; } = new(GameplayTags.Effect_Side_Swamp);

    /// <summary>
    /// Represents the Tailwind effect, which temporarily boosts the Speed of all Pokémon on the user's side of the field.
    /// </summary>
    /// <remarks>
    /// Doubles the Speed of all allied Pokémon on the user's side for 4 turns.
    /// The effect ends immediately if replaced or removed.
    /// Has no impact on Pokémon introduced after the effect is activated.
    /// </remarks>
    public static EffectHandle<FIntEffectData> Tailwind { get; } = new(GameplayTags.Effect_Side_Tailwind);

    /// <summary>
    /// Represents the Toxic Spikes effect, which lays a hazard on the battlefield that poisons opposing Pokémon upon switching in.
    /// </summary>
    /// <remarks>
    /// Toxic Spikes affect the opposing side of the battlefield. Pokémon switching in will be inflicted with the Poison effect, or badly poisoned if two layers of Toxic Spikes are active.
    /// Poison- or Steel-type Pokémon are unaffected and will remove the hazard if they are grounded and switch in.
    /// Does not affect Pokémon with the Levitate Ability, holding an Air Balloon, or those that are Flying-type unless the opposing team is grounded.
    /// </remarks>
    public static EffectHandle<FIntEffectData> ToxicSpikes { get; } = new(GameplayTags.Effect_Side_ToxicSpikes);

    /// <summary>
    /// Represents the Wide Guard effect, which protects the user's team from wide-range moves.
    /// </summary>
    /// <remarks>
    /// Prevents all allies from taking damage from moves that hit multiple targets during one turn.
    /// Does not protect against moves that target a single Pokémon or status moves.
    /// Cannot be used consecutively without failing unless certain conditions are met.
    /// </remarks>
    public static EffectHandle<FBoolEffectData> WideGuard { get; } = new(GameplayTags.Effect_Side_WideGuard);
}
