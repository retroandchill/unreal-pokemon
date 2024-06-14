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

void FBattleActionUseMove::Execute() {
    FBattleActionBase::Execute();
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
        FString::Format(TEXT("{0} used {1}!"), {
            GetBattler()->GetNickname().ToString(),
            IBattleMove::Execute_GetDisplayName(Move.GetObject()).ToString()
        }));
    GetOnActionFinished().ExecuteIfBound();
}

int32 FBattleActionUseMove::GetPriority() const {
    return IBattleMove::Execute_GetPriority(Move.GetObject());
}