using System.Collections.Immutable;
using System.Numerics;
using Pokemon.Battle.Entities;
using Pokemon.Battle.Services.Events.SpeedCalc;
using Pokemon.Data;
using Pokemon.Data.Model.HardCoded;
using UnrealInject.SourceGenerator.Attributes;

namespace Pokemon.Battle.Services.Stats;

/// <summary>
/// Provides stat calculation logic for a Pokémon battle context, including functionality to calculate stat stage changes
/// and effective speed for battlers. This implementation serves as the default service for handling battle stats.
/// </summary>
[Service]
public class DefaultBattleStatsService : IBattleStatsService
{
    private static readonly ImmutableArray<int> StatStageMultipliers = [2, 2, 2, 2, 2, 2, 2, 3, 4, 5, 6, 7, 8];
    private static readonly ImmutableArray<int> StatStageDivisors = [8, 7, 6, 5, 4, 3, 2, 2, 2, 2, 2, 2, 2];
    private static readonly ImmutableArray<int> AccEvaStageMultipliers = [3, 3, 3, 3, 3, 3, 3, 4, 5, 6, 7, 8, 9];
    private static readonly ImmutableArray<int> AccEvaStageDivisors = [9, 8, 7, 6, 5, 4, 3, 3, 3, 3, 3, 3, 3];

    /// <inheritdoc />
    public int StatStageMaximum => 6;

    private readonly ImmutableArray<ISpeedCalcEvaluator> _speedCalcEvaluators;

    /// <summary>
    /// Provides core logic for calculating Pokémon battle statistics,
    /// including stat stage adjustments and effective speed calculations for battlers.
    /// Implements the <see cref="IBattleStatsService"/> interface to standardize stat-related computations.
    /// </summary>
    public DefaultBattleStatsService(IEnumerable<ISpeedCalcEvaluator> speedCalcEvaluators)
    {
        _speedCalcEvaluators = [.. speedCalcEvaluators.OrderByDescending(x => x.Priority)];
    }

    /// <inheritdoc />
    public T CalculateStatStageChange<T>(FBattleStatHandle stat, T baseValue, int stage)
        where T : INumber<T>
    {
        var stageIndex = stage + StatStageMaximum;

        return stat.Entry.StatType switch
        {
            EStatType.Main => throw new InvalidOperationException("Main stats can't have stages"),
            EStatType.MainBattle => baseValue
                * T.CreateChecked(StatStageMultipliers[stageIndex])
                / T.CreateChecked(StatStageDivisors[stageIndex]),
            EStatType.Battle => baseValue
                * T.CreateChecked(AccEvaStageMultipliers[stageIndex])
                / T.CreateChecked(AccEvaStageDivisors[stageIndex]),
            _ => throw new InvalidOperationException("Invalid stat type"),
        };
    }

    /// <inheritdoc />
    public int GetEffectiveSpeed(UBattler battler)
    {
        var speed = CalculateStatStageChange(FStat.SPEED, battler.Speed, battler.GetStatStages(FStat.SPEED));

        var multiplier = 1.0f;
        foreach (var evaluator in _speedCalcEvaluators)
        {
            evaluator.Evaluate(battler, ref multiplier);
        }

        return Math.Max((int)Math.Round(speed * multiplier), 1);
    }
}
