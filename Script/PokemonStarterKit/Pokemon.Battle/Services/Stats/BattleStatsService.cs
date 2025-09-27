using Pokemon.Battle.Entities;

namespace Pokemon.Battle.Services.Stats;

public interface IBattleStatsService
{
    int GetEffectiveSpeed(UBattler battler);
}
