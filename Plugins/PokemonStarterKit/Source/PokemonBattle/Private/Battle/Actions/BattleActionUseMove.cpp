// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Actions/BattleActionUseMove.h"
#include "Battle/Battlers/Battler.h"
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

void FBattleActionUseMove::Execute() {
    FBattleActionBase::Execute();
    IBattleMove::Execute_PayCost(Move.GetObject());
}

FActionResult FBattleActionUseMove::ComputeResult() {
    FActionResult ActionResult = {.User = GetBattler()};
    int32 TargetCount = Targets.Num();
    for (const auto &Target : Targets) {
        if (Target->IsFainted()) {
            continue;
        }

        auto &[TargetValue, bHit, Damage, SecondaryEffects] = ActionResult.TargetResults.Emplace_GetRef();
        TargetValue = Target;
        bHit = IBattleMove::Execute_PerformHitCheck(Move.GetObject(), ActionResult.User, Target);
        if (!bHit) {
            // If the move misses then we don't want to apply any other effects
            continue;
        }

        Damage = IBattleMove::Execute_CalculateDamage(Move.GetObject(), ActionResult.User, Target, TargetCount);

        if (int32 EffectChance = IBattleMove::Execute_GetSecondaryEffectChance(Move.GetObject()); EffectChance == 0 || FMath::Rand() % 100 < EffectChance) {
            FSecondaryEffectHandle EffectHandle = {MakeShared<FAdditionalMoveEffects>()};
            IBattleMove::Execute_ApplySecondaryEffectsOnTarget(Move.GetObject(), Target, EffectHandle);
            SecondaryEffects = MoveTemp(*EffectHandle.Effects);
        }
    }

    if (int32 EffectChance = IBattleMove::Execute_GetSecondaryEffectChance(Move.GetObject()); EffectChance == 0 || FMath::Rand() % 100 < EffectChance) {
        FSecondaryEffectHandle EffectHandle = {MakeShared<FAdditionalMoveEffects>()};
        IBattleMove::Execute_ApplySecondaryEffectsOnUser(Move.GetObject(), ActionResult.User, EffectHandle);
        ActionResult.UserSecondaryEffects = MoveTemp(*EffectHandle.Effects);
    }
    

    return ActionResult;
}