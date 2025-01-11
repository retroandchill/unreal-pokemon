// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Actions/BattleActionDoNothing.h"

FBattleActionDoNothing::FBattleActionDoNothing(const TScriptInterface<IBattler> &BattlerIn) : Battler(BattlerIn) {
}

void FBattleActionDoNothing::AddReferencedObjects(FReferenceCollector &Collector) {
    Collector.AddReferencedObject(Battler.GetObjectRef());
}

FString FBattleActionDoNothing::GetReferencerName() const {
    return TEXT("FBattleActionDoNothing");
}

const TScriptInterface<IBattler> &FBattleActionDoNothing::GetBattler() const {
    return Battler;
}

bool FBattleActionDoNothing::CanExecute() const {
    return false;
}

UE5Coro::TCoroutine<> FBattleActionDoNothing::Execute() {
    // This will never get called
    co_return;
}

bool FBattleActionDoNothing::IsExecuting() const {
    return false;
}

bool FBattleActionDoNothing::IsComplete() const {
    return false;
}

int32 FBattleActionDoNothing::GetPriority() const {
    return 0;
}

FText FBattleActionDoNothing::GetActionMessage() const {
    return FText::GetEmpty();
}