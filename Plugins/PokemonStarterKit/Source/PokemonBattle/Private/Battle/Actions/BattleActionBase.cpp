// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Actions/BattleActionBase.h"
#include "Battle/Battlers/Battler.h"
#include <functional>

FBattleActionBase::FBattleActionBase(const TScriptInterface<IBattler> &BattlerIn) : Battler(BattlerIn) {
}

void FBattleActionBase::AddReferencedObjects(FReferenceCollector &Collector) {
    Collector.AddReferencedObject(Battler.GetObjectRef());
}

const TScriptInterface<IBattler> &FBattleActionBase::GetBattler() const {
    return Battler;
}

bool FBattleActionBase::CanExecute() const {
    return !Battler->IsFainted();
}

void FBattleActionBase::Execute() {
    Executing = true;
    Result = AsyncThread(std::bind_front(&FBattleActionBase::ComputeResult, this));
}

bool FBattleActionBase::IsExecuting() const {
    return Executing;
}

const TFuture<FActionResult> &FBattleActionBase::GetActionResult() const {
    return Result;
}