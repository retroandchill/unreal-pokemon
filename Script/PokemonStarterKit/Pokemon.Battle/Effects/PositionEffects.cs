using Pokemon.Battle.Actions;
using Pokemon.Data.Model.PBS;
using UnrealSharp.Attributes;

namespace Pokemon.Battle.Effects;

/// <summary>
/// Represents the data for the Future Sight move effect in battle.
/// </summary>
/// <param name="Counter">The counter value for the effect.</param>
/// <param name="Move">The move that is being used.</param>
/// <param name="User">The user of the move.</param>
/// <param name="Target">The target of the move.</param>
[UStruct]
public readonly partial record struct FFutureSightEffectData(
    [field: UProperty(PropertyFlags.BlueprintReadOnly)] int Counter,
    [field: UProperty(PropertyFlags.BlueprintReadOnly)] FMoveHandle Move,
    [field: UProperty(PropertyFlags.BlueprintReadOnly)] FActionTarget User,
    [field: UProperty(PropertyFlags.BlueprintReadOnly)] FActionTarget Target
)
{
    /// <summary>
    /// Represents the default or "no value" state for the <see cref="FFutureSightEffectData"/> struct.
    /// </summary>
    /// <remarks>
    /// This property initializes an empty or uninitialized state for Future Sight effect data.
    /// The Counter will default to 0, and both Move and Target properties will default to their "None" values.
    /// </remarks>
    public static FFutureSightEffectData None => new(0, FMoveHandle.None, FActionTarget.None, FActionTarget.None);
}

/// <summary>
/// Represents the data for the Wish move effect in battle.
/// </summary>
/// <param name="Counter">The counter value for the effect, representing the number of turns remaining.</param>
/// <param name="Amount">The amount of HP to be restored when Wish activates.</param>
/// <param name="Maker">The action target who initiated the Wish effect.</param>
[UStruct]
public readonly partial record struct FWishEffectData(
    [field: UProperty(PropertyFlags.BlueprintReadOnly)] int Counter,
    [field: UProperty(PropertyFlags.BlueprintReadOnly)] int Amount,
    [field: UProperty(PropertyFlags.BlueprintReadOnly)] FActionTarget Maker
)
{
    /// <summary>
    /// Provides a predefined "no value" instance of the <see cref="FWishEffectData"/> struct.
    /// </summary>
    /// <remarks>
    /// This property represents an uninitialized state for the Wish effect data.
    /// The Counter and Amount values default to 0, and the Maker property defaults to <see cref="FActionTarget.None"/>.
    /// </remarks>
    public static FWishEffectData None => new(0, 0, FActionTarget.None);
}

/// <summary>
/// Provides a collection of effect handles associated with different positional effects in battle.
/// </summary>
/// <remarks>
/// Contains predefined handles for effects such as Future Sight, Healing Wish, Lunar Dance, and Wish,
/// including their default values for initialization.
/// </remarks>
public static class PositionEffects
{
    /// <summary>
    /// Represents the effect handle for the Future Sight positional effect in battle.
    /// </summary>
    /// <remarks>
    /// This property provides the predefined handle associated with the Future Sight effect,
    /// including its default initialization state using <see cref="FFutureSightEffectData.None"/>.
    /// The associated tag for this effect is identified as "Effect.Position.FutureSight".
    /// </remarks>
    public static EffectHandle<FFutureSightEffectData> FutureSight { get; } =
        new(GameplayTags.Effect_Position_FutureSight, FFutureSightEffectData.None);

    /// <summary>
    /// Represents the effect handle for the "Healing Wish" positional effect in battle.
    /// </summary>
    /// <remarks>
    /// This property is associated with the Healing Wish effect, which utilizes <see cref="FBoolEffectData"/> to represent its state.
    /// It is identified by the gameplay tag "Effect.Position.HealingWish" and does not define a specific default value.
    /// </remarks>
    public static EffectHandle<FBoolEffectData> HealingWish { get; } = new(GameplayTags.Effect_Position_HealingWish);

    /// <summary>
    /// Represents the effect handle for the Lunar Dance positional effect in a Pokémon battle.
    /// </summary>
    /// <remarks>
    /// This property defines an effect associated with the Lunar Dance move, utilizing the <see cref="FBoolEffectData"/> structure.
    /// The effect is identified by the gameplay tag <c>Effect.Position.LunarDance</c>.
    /// </remarks>
    public static EffectHandle<FBoolEffectData> LunarDance { get; } = new(GameplayTags.Effect_Position_LunarDance);

    /// <summary>
    /// Represents the handle for the "Wish" positional effect in a battle.
    /// </summary>
    /// <remarks>
    /// This property provides access to the Wish effect, using a predefined gameplay tag and the default
    /// value of <see cref="FWishEffectData"/>. The Wish effect is typically associated with healing mechanics
    /// in a battle scenario, tracked through its countdown and associated parameters.
    /// </remarks>
    public static EffectHandle<FWishEffectData> Wish { get; } =
        new(GameplayTags.Effect_Position_Wish, FWishEffectData.None);
}
