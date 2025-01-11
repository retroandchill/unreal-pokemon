// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Actions/BattleActionBase.h"
#include "Battle/Battlers/Battler.h"
#include "Battle/Battlers/BattlerAbilityComponent.h"
#include "Battle/BattleSide.h"
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
    return !Battler->IsFainted() && Battler->GetOwningSide()->GetBattlers().Contains(Battler);
}

UE5Coro::TCoroutine<> FBattleActionBase::Execute() {
    Executing = true;
    SpecHandle = ActivateAbility();
    co_await UE5Coro::Latent::Until(Retro::BindMethod<&FBattleActionBase::IsComplete>(this));
}

bool FBattleActionBase::IsExecuting() const {
    return Executing;
}

bool FBattleActionBase::IsComplete() const {
    auto Spec = Battler->GetAbilityComponent()->FindAbilitySpecFromHandle(SpecHandle);
    return Executing && Spec != nullptr && !Spec->IsActive();
}