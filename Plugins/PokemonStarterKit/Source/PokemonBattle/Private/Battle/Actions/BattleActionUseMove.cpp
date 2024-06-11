// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/Actions/BattleActionUseMove.h"


FBattleActionUseMove::FBattleActionUseMove(const TScriptInterface<IBattler> &BattlerIn,
    const TScriptInterface<IBattleMove> &MoveIn) : FBattleActionBase(BattlerIn), Move(MoveIn) {
}

void FBattleActionUseMove::AddReferencedObjects(FReferenceCollector &Collector) {
    FBattleActionBase::AddReferencedObjects(Collector);
    Collector.AddReferencedObject(Move.GetObjectRef());
}

FString FBattleActionUseMove::GetReferencerName() const {
    return TEXT("FBattleActionUseMove");
}