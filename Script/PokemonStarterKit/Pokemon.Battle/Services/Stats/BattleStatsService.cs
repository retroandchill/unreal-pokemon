using System.Numerics;
using Pokemon.Battle.Entities;
using Pokemon.Data.Model.HardCoded;

namespace Pokemon.Battle.Services.Stats;

public interface IBattleStatsService
{
    int StatStageMaximum { get; }

    T CalculateStatStageChange<T>(FBattleStatHandle stat, T baseValue, int stage)
        where T : INumber<T>;

    int GetEffectiveSpeed(UBattler battler);
}
