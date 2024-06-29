// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Actions/BattleActionUseMove.h"
#include "Abilities/GameplayAbility.h"
#include "Battle/Battlers/Battler.h"
#include "Battle/GameplayAbilities/BattlerAbilityComponent.h"
#include "Battle/GameplayAbilities/Context/MoveEffectContext.h"
#include "Battle/Moves/BattleMove.h"
#include "Battle/Moves/MoveTags.h"

FBattleActionUseMove::FBattleActionUseMove(const TScriptInterface<IBattler> &BattlerIn,
                                           const TScriptInterface<IBattleMove> &MoveIn,
                                           TArray<TScriptInterface<IBattler>> &&TargetsIn)
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

FGameplayAbilitySpecHandle FBattleActionUseMove::ActivateAbility() {
    return Move->TryActivateMove(Targets);
}