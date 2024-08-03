﻿// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Actions/BattleActionUseMove.h"
#include "Abilities/GameplayAbility.h"
#include "Battle/Attributes/PokemonCoreAttributeSet.h"
#include "Battle/Battlers/Battler.h"
#include "Battle/Battlers/BattlerAbilityComponent.h"
#include "Battle/Moves/BattleMove.h"

FBattleActionUseMove::FBattleActionUseMove(const TScriptInterface<IBattler> &BattlerIn,
                                           const TScriptInterface<IBattleMove> &MoveIn,
                                           TArray<FTargetWithIndex> &&TargetsIn)
    : FBattleActionBase(BattlerIn), Move(MoveIn), Targets(MoveTemp(TargetsIn)) {
}

void FBattleActionUseMove::AddReferencedObjects(FReferenceCollector &Collector) {
    FBattleActionBase::AddReferencedObjects(Collector);
    Collector.AddReferencedObject(Move.GetObjectRef());
}

FString FBattleActionUseMove::GetReferencerName() const {
    return TEXT("FBattleActionUseMove");
}

int32 FBattleActionUseMove::GetPriority() const {
    return Move->GetPriority();
}

FText FBattleActionUseMove::GetActionMessage() const {
    return FText::Format(FText::FromStringView(TEXT("{0} used {1}!")),
                         {GetBattler()->GetNickname(), Move->GetDisplayName()});
}

void FBattleActionUseMove::Execute() {
    FBattleActionBase::Execute();

    auto AttributeSet = GetBattler()->GetAbilityComponent()->GetCoreAttributes();
    check(AttributeSet != nullptr)
    Move->PayCost(FMath::FloorToInt32(AttributeSet->GetMoveCost()));
}

FGameplayAbilitySpecHandle FBattleActionUseMove::ActivateAbility() {
    return Move->TryActivateMove(Targets);
}