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

void FBattleActionBase::Execute(bool bPerformAsync) {
    Executing = true;
    if (bPerformAsync) {
        Result = Async(EAsyncExecution::TaskGraphMainThread, std::bind_front(&FBattleActionBase::ComputeResult, this));
    } else {
        TPromise<FActionResult> Promise;
        Promise.EmplaceValue(ComputeResult());
        Result = Promise.GetFuture();
    }
    
}

bool FBattleActionBase::IsExecuting() const {
    return Executing;
}

const TFuture<FActionResult> &FBattleActionBase::GetActionResult() const {
    return Result;
}