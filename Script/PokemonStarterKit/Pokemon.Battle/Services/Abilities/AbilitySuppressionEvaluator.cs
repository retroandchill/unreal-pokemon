using Pokemon.Battle.Entities;
using Pokemon.Data.Model.PBS;

namespace Pokemon.Battle.Services.Abilities;

public interface IAbilitySuppressionEvaluator
{
    bool IsAbilitySuppressed(UBattler battler, FAbilityHandle checkAbility = default);
}
