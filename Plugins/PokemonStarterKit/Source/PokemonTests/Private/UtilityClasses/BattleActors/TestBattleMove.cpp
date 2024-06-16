﻿// "Unreal Pokémon" created by Retro & Chill.

#include "TestBattleMove.h"

TScriptInterface<IBattleMove> UTestBattleMove::Initialize(const TScriptInterface<IBattle> &Battle,
                                                          const TScriptInterface<IMove> &Move) {
    return this;
}

bool UTestBattleMove::IsConfusionAttack() const {
    return true;
}

bool UTestBattleMove::HasTag(FName Tag) const {
    return false;
}

FText UTestBattleMove::GetDisplayName_Implementation() const {
    return FText::FromStringView(TEXT("Test Move"));
}

void UTestBattleMove::PayCost_Implementation() {
    CostsPaid += 1;
}

FBattleDamage UTestBattleMove::CalculateDamage_Implementation(const TScriptInterface<IBattler> &User,
                                                              const TScriptInterface<IBattler> &Target,
                                                              int32 TargetCount) {
    return {.Damage = 20};
}