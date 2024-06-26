// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Actions/BattleActionUseMove.h"
#include "Abilities/GameplayAbility.h"
#include "Battle/Battlers/Battler.h"
#include "Battle/GameplayAbilities/BattlerAbilityComponent.h"
#include "Battle/GameplayAbilities/Context/MoveEffectContext.h"
#include "Battle/Moves/BattleMove.h"

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
    return IBattleMove::Execute_GetPriority(Move.GetObject());
}

FText FBattleActionUseMove::GetActionMessage() const {
    return FText::Format(FText::FromStringView(TEXT("{0} used {1}!")),
                         {GetBattler()->GetNickname(), IBattleMove::Execute_GetDisplayName(Move.GetObject())});
}

void FBattleActionUseMove::Execute(bool bPerformAsync) {
    FBattleActionBase::Execute(bPerformAsync);
    IBattleMove::Execute_PayCost(Move.GetObject());
}

FActionResult FBattleActionUseMove::ComputeResult() {
    const static auto UseMoveTag = FGameplayTag::RequestGameplayTag("Battle.UsingMove");
    FActionResult ActionResult;
    auto &User = GetBattler();
    User->GetAbilityComponent()->AddLooseGameplayTag(UseMoveTag);
    int32 TargetCount = Targets.Num();
    for (const auto &Target : Targets) {
        if (Target->IsFainted()) {
            continue;
        }
        
        auto &TargetResult = ActionResult.TargetResults.Emplace_GetRef();
        TargetResult.Target = Target;
        TargetResult.bHit = IBattleMove::Execute_PerformHitCheck(Move.GetObject(), User, Target);
        if (!TargetResult.bHit) {
            // If the move misses then we don't want to apply any other effects
            continue;
        }
        
        TargetResult.Damage = IBattleMove::Execute_CalculateDamage(Move.GetObject(), User, Target, TargetCount);
    }

    User->GetAbilityComponent()->RemoveLooseGameplayTag(UseMoveTag);
    return ActionResult;
}