namespace Pokemon.Battle.Effects;

/// <summary>
/// Represents a collection of predefined field effects in a Pokémon battle.
/// Each field effect is represented as a static property exposing an <see cref="EffectHandle{T}"/>
/// to handle specific game mechanics or conditions related to the battle field.
/// </summary>
public static class FieldEffects
{
    /// <summary>
    /// Represents the "Amulet Coin" field effect in a Pokémon battle. When active, this effect
    /// increases the amount of money earned after a battle, typically doubling the prize winnings.
    /// This effect is usually associated with the Amulet Coin item.
    /// </summary>
    /// <remarks>
    /// The "Amulet Coin" effect is stored as a boolean value, indicating whether the effect is active
    /// or inactive during the battle.
    /// </remarks>
    public static EffectHandle<FBoolEffectData> AmuletCoin { get; } = new(GameplayTags.Effect_Field_AmuletCoin);

    /// <summary>
    /// Represents the "Fairy Lock" field effect in a Pokémon battle. This effect prevents Pokémon from
    /// fleeing or being swapped out during battle for a limited duration, typically tied to the use
    /// of the move Fairy Lock or similar effects.
    /// </summary>
    /// <remarks>
    /// The "Fairy Lock" effect is stored as an integer value, which may represent the remaining duration
    /// of the effect or other contextual data. It impacts both sides of the battlefield while active.
    /// </remarks>
    public static EffectHandle<FIntEffectData> FairyLock { get; } = new(GameplayTags.Effect_Field_FairyLock);

    /// <summary>
    /// Represents the "Fusion Bolt" field effect in a Pokémon battle. When active, this effect
    /// signifies the presence of the Fusion Bolt move's unique interaction on the battle field,
    /// often tied to synergistic effects with the move Fusion Flare.
    /// </summary>
    /// <remarks>
    /// The "Fusion Bolt" effect is stored as a boolean value, representing whether the effect
    /// is active or inactive during the battle.
    /// </remarks>
    public static EffectHandle<FBoolEffectData> FusionBolt { get; } = new(GameplayTags.Effect_Field_FusionBolt);

    /// <summary>
    /// Represents the "Fusion Flare" field effect in a Pokémon battle. When active, this effect
    /// signifies the presence of a Fusion Flare attack's lingering impact, potentially
    /// interacting with other abilities or moves, such as Fusion Bolt, to produce enhanced effects.
    /// </summary>
    /// <remarks>
    /// The "Fusion Flare" effect is stored as a boolean value, indicating whether the effect
    /// is active or inactive during the battle field state.
    /// </remarks>
    public static EffectHandle<FBoolEffectData> FusionFlare { get; } = new(GameplayTags.Effect_Field_FusionFlare);

    /// <summary>
    /// Represents the "Gravity" field effect in a Pokémon battle. When active, this effect
    /// intensifies gravitational forces on the battlefield, preventing the use of certain moves
    /// such as those involving flight or bouncing. Additionally, Pokémon are unable to evade
    /// moves due to increased precision caused by the effect.
    /// </summary>
    /// <remarks>
    /// The "Gravity" effect is stored as an integer value, typically representing the number of turns
    /// the effect remains active on the battlefield.
    /// </remarks>
    public static EffectHandle<FIntEffectData> Gravity { get; } = new(GameplayTags.Effect_Field_Gravity);

    /// <summary>
    /// Represents the "Happy Hour" field effect in a Pokémon battle. When active, this effect
    /// increases the prize money earned after the battle, typically multiplying the rewards.
    /// This effect is commonly triggered by the use of the move "Happy Hour."
    /// </summary>
    /// <remarks>
    /// The "Happy Hour" effect is stored as a boolean value, indicating whether the effect is active
    /// or inactive during the battle.
    /// </remarks>
    public static EffectHandle<FBoolEffectData> HappyHour { get; } = new(GameplayTags.Effect_Field_HappyHour);

    /// <summary>
    /// Represents the "Ion Deluge" field effect in a Pokémon battle. This effect converts all Normal-type moves
    /// used during its activation into Electric-type moves, interacting with Electric-type Pokémon and moves.
    /// </summary>
    /// <remarks>
    /// The "Ion Deluge" effect is stored as a boolean value, indicating whether the effect is active or inactive
    /// during the battle. It modifies the typing of moves dynamically, adding a unique tactical element to battles.
    /// </remarks>
    public static EffectHandle<FBoolEffectData> IonDeluge { get; } = new(GameplayTags.Effect_Field_IonDeluge);

    /// <summary>
    /// Represents the "Magic Room" field effect in a Pokémon battle. While active, this effect
    /// suppresses the effects of all held items for the duration of its influence.
    /// </summary>
    /// <remarks>
    /// The "Magic Room" effect is stored as an integer value, typically used to track its active duration
    /// or remaining turns during the battle.
    /// </remarks>
    public static EffectHandle<FIntEffectData> MagicRoom { get; } = new(GameplayTags.Effect_Field_MagicRoom);

    /// <summary>
    /// Represents the "Mud Sport" field effect in a Pokémon battle. When active, this effect
    /// reduces the power of Electric-type moves for all Pokémon on the field.
    /// </summary>
    /// <remarks>
    /// The "Mud Sport" effect is stored as an integer value, typically corresponding to its duration
    /// or intensity. It is often triggered by moves or abilities during battle and affects the
    /// effectiveness of Electric-type moves globally.
    /// </remarks>
    public static EffectHandle<FIntEffectData> MudSport { get; } = new(GameplayTags.Effect_Field_MudSport);

    /// <summary>
    /// Represents the "Pay Day" field effect in a Pokémon battle. When active, this effect
    /// determines the additional in-game currency earned after a battle as a result of using the move "Pay Day."
    /// The value typically increases depending on factors like the move's power and battle context.
    /// </summary>
    /// <remarks>
    /// The "Pay Day" effect is stored as a numerical value, which indicates the total amount of extra currency
    /// generated due to the effect during the battle. This effect is often associated with the move "Pay Day."
    /// </remarks>
    public static EffectHandle<FIntEffectData> PayDay { get; } = new(GameplayTags.Effect_Field_PayDay);

    /// <summary>
    /// Represents the "Trick Room" field effect in a Pokémon battle. When active, this effect reverses
    /// the speed priority of all Pokémon on the battlefield, causing slower Pokémon to move first.
    /// This effect typically lasts for a defined number of turns.
    /// </summary>
    /// <remarks>
    /// The "Trick Room" effect is associated with an integer value that determines the remaining duration
    /// of the effect in turns. When the value reaches zero, the effect ends, restoring normal turn order
    /// based on speed stats.
    /// </remarks>
    public static EffectHandle<FIntEffectData> TrickRoom { get; } = new(GameplayTags.Effect_Field_TrickRoom);

    /// <summary>
    /// Represents the "Water Sport" field effect in a Pokémon battle. When active, this effect
    /// reduces the power of Fire-type moves used by any Pokémon on the battlefield.
    /// </summary>
    /// <remarks>
    /// The "Water Sport" effect is stored as an integer value which may indicate its duration or intensity
    /// in the battle, depending on game-specific mechanics.
    /// </remarks>
    public static EffectHandle<FIntEffectData> WaterSport { get; } = new(GameplayTags.Effect_Field_WaterSport);

    /// <summary>
    /// Represents the "Wonder Room" field effect in a Pokémon battle. This effect swaps the Defense
    /// and Special Defense stats of all Pokémon on the battlefield for a certain duration.
    /// </summary>
    /// <remarks>
    /// The "Wonder Room" effect is stored as an integer value, commonly used to indicate the
    /// remaining duration of the effect in turns. When the effect expires, the stats revert to normal.
    /// </remarks>
    public static EffectHandle<FIntEffectData> WonderRoom { get; } = new(GameplayTags.Effect_Field_WonderRoom);
}
