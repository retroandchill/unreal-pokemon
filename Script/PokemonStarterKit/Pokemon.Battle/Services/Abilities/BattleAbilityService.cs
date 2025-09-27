using Pokemon.Battle.Entities;
using Pokemon.Data.Model.PBS;

namespace Pokemon.Battle.Services.Abilities;

public interface IBattleAbilityService
{
    bool HasActiveAbility(
        UBattler battler,
        bool ignoreFained = false,
        FAbilityHandle checkAbility = default
    );
}
