using Pokemon.Battle.Entities;
using Pokemon.Battle.Services.Events.SpeedCalc;
using UnrealInject.SourceGenerator.Attributes;

namespace Pokemon.Battle.Effects.Side;

/// <summary>
/// Represents the Tailwind effect in a battle context, implementing the ISpeedCalcEvaluator interface.
/// This effect is responsible for modifying the speed calculation of a battler during battle.
/// </summary>
[Service]
public class TailwindEffect : ISpeedCalcEvaluator
{
    int ISpeedCalcEvaluator.Priority => SpeedCalcPriorities.Tailwind;

    void ISpeedCalcEvaluator.Evaluate(UBattler battler, ref float multiplier)
    {
        if (battler.OwningSide.GetEffect(SideEffects.Tailwind) > 0)
        {
            multiplier *= 2;
        }
    }
}
