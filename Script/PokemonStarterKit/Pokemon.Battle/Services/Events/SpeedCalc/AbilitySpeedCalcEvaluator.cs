using Pokemon.Battle.Entities;
using Pokemon.Battle.Events;
using Retro.ReadOnlyParams.Annotations;

namespace Pokemon.Battle.Services.Events.SpeedCalc;

public class AbilitySpeedCalcEvaluator([ReadOnly] AbilityEventHandlers handlers)
    : ISpeedCalcEvaluator
{
    public int Priority => 10;

    public void Evaluate(UBattler battler, ref float multiplier)
    {
        if (battler is { AbilityActive: true, Ability: not null })
        {
            multiplier = handlers.TriggerSpeedCalc(battler.Ability.Value, battler, multiplier);
        }
    }
}
