// "Unreal Pokémon" created by Retro & Chill.

#include "TestBattleMove.h"
#include "Moves/Target.h"

TScriptInterface<IBattleMove> UTestBattleMove::Initialize(const TScriptInterface<IBattle> &Battle,
                                                          const TScriptInterface<IMove> &Move) {
    return this;
}

const FMoveTarget & UTestBattleMove::GetTargetType() const {
    static FMoveTarget Target;
    return Target;
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

int32 UTestBattleMove::GetBasePower() const {
    return 0;
}

int32 UTestBattleMove::GetAccuracy() const {
    return 0;
}

const TArray<FName> & UTestBattleMove::GetTags() const {
    static TArray<FName> Names;
    return Names;
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