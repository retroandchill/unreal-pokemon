using Pokemon.Battle.Entities;
using Pokemon.Battle.Events;
using Retro.ReadOnlyParams.Annotations;
using UnrealInject.SourceGenerator.Attributes;

namespace Pokemon.Battle.Services.Events.SpeedCalc;

[Service]
public class ItemSpeedCalcEvaluator([ReadOnly] AbilityEventHandlers handlers)
    : ISpeedCalcEvaluator
{
    public int Priority => SpeedCalcPriorities.Ability;

    public void Evaluate(UBattler battler, ref float multiplier)
    {
        if (battler is { AbilityActive: true, Ability: not null })
        {
            multiplier = handlers.TriggerSpeedCalc(battler.Ability.Value, battler, multiplier);
        }
    }
}
