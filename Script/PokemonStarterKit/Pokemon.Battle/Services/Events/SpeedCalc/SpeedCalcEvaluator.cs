using Pokemon.Battle.Entities;

namespace Pokemon.Battle.Services.Events.SpeedCalc;

public interface ISpeedCalcEvaluator
{
    int Priority { get; }

    void Evaluate(UBattler battler, ref float multiplier);
}
