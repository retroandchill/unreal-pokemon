// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Actions/BattleActionUseMove.h"
#include "Battle/Battlers/Battler.h"
#include "Battle/Moves/BattleMove.h"

FBattleActionUseMove::FBattleActionUseMove(const TScriptInterface<IBattler> &BattlerIn, const TScriptInterface<IBattleMove> &MoveIn, TArray<TScriptInterface<IBattler>> &&TargetsIn) : FBattleActionBase(BattlerIn),Move(MoveIn), Targets(MoveTemp(TargetsIn)) {
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
    return FText::Format(FText::FromStringView(TEXT("{0} used {1}!")), {
        GetBattler()->GetNickname(),
        IBattleMove::Execute_GetDisplayName(Move.GetObject())
    });
}

FActionResult FBattleActionUseMove::ComputeResult() {
    FActionResult Result;

    auto &User = GetBattler();
    int32 TargetCount = Targets.Num();
    for (auto &Target : Targets) {
        auto &TargetResult = Result.TargetResults.Emplace_GetRef();
        TargetResult.Target = Target;
        TargetResult.bHit = true; // Everything will hit for now
        TargetResult.Damage = IBattleMove::Execute_CalculateDamage(Move.GetObject(), User, Target, TargetCount);
    }
    
    return Result;
}