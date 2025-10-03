using Pokemon.Battle.Entities;

namespace Pokemon.Battle.Services.Events.SpeedCalc;

/// <summary>
/// Defines the interface for evaluating speed calculation modifications in a battle context.
/// Implementations of this interface modify the speed multiplier for a battler based on specific conditions or effects.
/// </summary>
public interface ISpeedCalcEvaluator
{
    /// <summary>
    /// Represents the priority of the speed calculation evaluator within the ordering system.
    /// Priority determines the evaluation order among multiple speed calculation modifiers, with higher values being executed first.
    /// Typically used in scenarios where different effects or conditions modify battler speed, ensuring the modifications apply in a specific sequence.
    /// </summary>
    int Priority { get; }

    /// <summary>
    /// Modifies the speed multiplier of a battler based on specific conditions or effects.
    /// Implementations of this method apply changes to the speed multiplier during speed calculations in battle.
    /// </summary>
    /// <param name="battler">
    /// The battler whose speed is being evaluated. Represents a combatant in the battle.
    /// </param>
    /// <param name="multiplier">
    /// The current speed multiplier which is subject to modification. This is passed by reference to allow alterations.
    /// </param>
    void Evaluate(UBattler battler, ref float multiplier);
}
