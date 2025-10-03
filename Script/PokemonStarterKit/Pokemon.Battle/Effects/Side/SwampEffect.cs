using Pokemon.Battle.Entities;
using Pokemon.Battle.Services.Events.SpeedCalc;
using UnrealInject.SourceGenerator.Attributes;

namespace Pokemon.Battle.Effects.Side;

/// <summary>
/// Represents an effect applied within a Pokémon battle that influences speed calculations for battlers.
/// </summary>
/// <remarks>
/// This class implements the <see cref="ISpeedCalcEvaluator"/> interface, enabling it to contribute
/// to the speed determination of Pokémon during battle.
/// </remarks>
[Service]
public class SwampEffect : ISpeedCalcEvaluator
{
    int ISpeedCalcEvaluator.Priority => SpeedCalcPriorities.Swamp;

    void ISpeedCalcEvaluator.Evaluate(UBattler battler, ref float multiplier)
    {
        if (battler.OwningSide.GetEffect(SideEffects.Swamp) > 0)
        {
            multiplier /= 2;
        }
    }
}
