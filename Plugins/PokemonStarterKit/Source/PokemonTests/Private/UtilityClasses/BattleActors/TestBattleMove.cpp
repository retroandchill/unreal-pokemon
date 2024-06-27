// "Unreal Pokémon" created by Retro & Chill.

#include "TestBattleMove.h"

TScriptInterface<IBattleMove> UTestBattleMove::Initialize(const TScriptInterface<IBattle> &Battle,
                                                          const TScriptInterface<IMove> &Move) {
    return this;
}

bool UTestBattleMove::IsConfusionAttack() const {
    return false;
}

bool UTestBattleMove::HasHighCriticalHitRate() const {
    return false;
}

bool UTestBattleMove::HasTag(FName Tag) const {
    return false;
}

FText UTestBattleMove::GetDisplayName_Implementation() const {
    return FText::FromStringView(TEXT("Test Move"));
}

void UTestBattleMove::PayCost_Implementation(int32 Amount) {
    CostsPaid += Amount;
}

bool UTestBattleMove::PerformHitCheck_Implementation(const TScriptInterface<IBattler> &User,
    const TScriptInterface<IBattler> &Target) {
    return true;
}

FBattleDamage UTestBattleMove::CalculateDamage_Implementation(const TScriptInterface<IBattler> &User,
                                                              const TScriptInterface<IBattler> &Target,
                                                              int32 TargetCount) {
    return {.Damage = 20};
}