// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/Actions/BattleActionBase.h"

FBattleActionBase::FBattleActionBase(const TScriptInterface<IBattler> &BattlerIn) : Battler(BattlerIn) {
}

void FBattleActionBase::AddReferencedObjects(FReferenceCollector &Collector) {
    Collector.AddReferencedObject(Battler.GetObjectRef());
}

const TScriptInterface<IBattler> & FBattleActionBase::GetBattler() const {
    return Battler;
}