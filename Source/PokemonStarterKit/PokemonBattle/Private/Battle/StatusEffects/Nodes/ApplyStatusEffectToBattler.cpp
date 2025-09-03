// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/StatusEffects/Nodes/ApplyStatusEffectToBattler.h"
#include "Battle/StatusEffects/BattleStatusEffectUtils.h"

UApplyStatusEffectToBattler *
UApplyStatusEffectToBattler::ApplyStatusEffectToBattler(const TScriptInterface<IBattler> &Target,
                                                        FStatusHandle StatusEffectID) {
    auto Node = NewObject<UApplyStatusEffectToBattler>();
    Node->SetWorldContext(Target.GetObject());
    Node->Target = Target;
    Node->StatusEffectID = StatusEffectID;
    return Node;
}

UE5Coro::TCoroutine<> UApplyStatusEffectToBattler::ExecuteCoroutine(FForceLatentCoroutine ForceLatentCoroutine) {
    OnComplete.Broadcast(co_await UBattleStatusEffectUtils::ApplyStatusEffectToBattler(Target, StatusEffectID));
}