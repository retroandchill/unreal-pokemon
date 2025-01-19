// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/StatusEffects/Nodes/RemoveStatusEffectFromBattler.h"
#include "Battle/StatusEffects/BattleStatusEffectUtils.h"

URemoveStatusEffectFromBattler * URemoveStatusEffectFromBattler::RemoveStatusEffectFromBattler(
    const TScriptInterface<IBattler> &Target, FStatusHandle StatusEffectID) {
    auto Node = NewObject<URemoveStatusEffectFromBattler>();
    Node->SetWorldContext(Target.GetObject());
    Node->Target = Target;
    Node->StatusEffectID = StatusEffectID;
    return Node;
}

UE5Coro::TCoroutine<> URemoveStatusEffectFromBattler::ExecuteCoroutine(FForceLatentCoroutine ForceLatentCoroutine) {
    OnComplete.Broadcast(co_await UBattleStatusEffectUtils::RemoveStatusEffectFromBattler(Target, StatusEffectID));
}