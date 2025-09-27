using System.Collections.Immutable;
using Pokemon.Battle.Entities;
using Pokemon.Battle.Services.Events.SpeedCalc;

namespace Pokemon.Battle.Services.Stats;

public class DefaultBattleStatsService : IBattleStatsService
{
    private readonly ImmutableArray<ISpeedCalcEvaluator> _speedCalcEvaluators;

    public DefaultBattleStatsService(IEnumerable<ISpeedCalcEvaluator> speedCalcEvaluators)
    {
        _speedCalcEvaluators = [.. speedCalcEvaluators.OrderByDescending(x => x.Priority)];
    }

    public int GetEffectiveSpeed(UBattler battler)
    {
        var speed = battler.Speed;

        var multiplier = 1.0f;
        foreach (var evaluator in _speedCalcEvaluators)
        {
            evaluator.Evaluate(battler, ref multiplier);
        }

        return Math.Max((int)Math.Round(speed * multiplier), 1);
    }
}
