// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/Moves/BaseBattleMove.h"
#include "Battle/Battlers/Battler.h"
#include "Pokemon/Moves/Move.h"

TScriptInterface<IBattleMove> UBaseBattleMove::Initialize(const TScriptInterface<IMove> &Move) {
    WrappedMove = Move;
    return this;
}

int32 UBaseBattleMove::CalculateBaseDamage_Implementation(const TScriptInterface<IBattler> &User,
    const TScriptInterface<IBattler> &Target) {
    auto [Attack, Defense] = Execute_GetAttackAndDefense(this, User, Target);
    int32 Power = Execute_CalculateTruePower(this, User, Target);
    return (2 * User->GetLevel() / 5 + 2) * Power * Attack / Defense / 50 + 2;
}

int32 UBaseBattleMove::CalculateTruePower_Implementation(const TScriptInterface<IBattler> &User,
    const TScriptInterface<IBattler> &Target) {
    return WrappedMove->GetBasePower();
}

FAttackAndDefense UBaseBattleMove::GetAttackAndDefense_Implementation(const TScriptInterface<IBattler> &User,
    const TScriptInterface<IBattler> &Target) {
    using enum EMoveDamageCategory;
    auto Category = WrappedMove->GetDamageCategory();
    check(Category != Status)
    return Category == Special ? FAttackAndDefense{ .Attack = User->GetSpecialAttack(), .Defense = Target->GetSpecialDefense() }
        : FAttackAndDefense{ .Attack = User->GetAttack(), .Defense = Target->GetDefense() };
}