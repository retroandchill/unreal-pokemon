using Pokemon.Battle.Entities;

namespace Pokemon.Battle.Events;

/// <summary>
/// Represents a delegate for calculating the speed of a battler in the battle system.
/// </summary>
/// <param name="battler">
/// The battler whose speed is to be calculated. This parameter provides access
/// to the battler's components, stats, abilities, and current state in the battle context.
/// </param>
/// <param name="multiplier">
/// An optional multiplier applied to the calculated speed. Defaults to 1.0f
/// if not specified.
/// </param>
/// <returns>
/// A float value representing the calculated speed of the provided battler,
/// modified by the given multiplier.
/// </returns>
public delegate float SpeedCalc(UBattler battler, float multiplier = 1.0f);
