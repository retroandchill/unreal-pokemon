using Pokemon.Battle.Entities;
using Pokemon.Battle.Events;
using Retro.ReadOnlyParams.Annotations;
using UnrealInject.SourceGenerator.Attributes;

namespace Pokemon.Battle.Services.Events.SpeedCalc;

[Service]
public class AbilitySpeedCalcEvaluator([ReadOnly] ItemEventHandlers handlers) : ISpeedCalcEvaluator
{
    public int Priority => SpeedCalcPriorities.Item;

    public void Evaluate(UBattler battler, ref float multiplier)
    {
        if (battler is { ItemActive: true, HoldItem: not null })
        {
            multiplier = handlers.TriggerSpeedCalc(battler.HoldItem.Value, battler, multiplier);
        }
    }
}
