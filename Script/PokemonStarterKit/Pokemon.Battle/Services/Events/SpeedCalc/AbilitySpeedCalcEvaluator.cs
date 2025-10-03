using Pokemon.Battle.Entities;
using Pokemon.Battle.Events;
using Retro.ReadOnlyParams.Annotations;
using UnrealInject.SourceGenerator.Attributes;

namespace Pokemon.Battle.Services.Events.SpeedCalc;

/// <summary>
/// Evaluates and modifies the speed multiplier for a battler based on its held item and active item state.
/// </summary>
/// <param name="handlers">The event handlers for ability-based speed calculations.</param>"
[Service]
public class AbilitySpeedCalcEvaluator([ReadOnly] ItemEventHandlers handlers) : ISpeedCalcEvaluator
{
    /// <inheritdoc />
    public int Priority => SpeedCalcPriorities.Item;

    /// <inheritdoc />
    public void Evaluate(UBattler battler, ref float multiplier)
    {
        if (battler is { ItemActive: true, HoldItem: not null })
        {
            multiplier = handlers.TriggerSpeedCalc(battler.HoldItem.Value, battler, multiplier);
        }
    }
}
