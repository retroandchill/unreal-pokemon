// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/StatusEffects/Nodes/CanStatusEffectBeInflicted.h"
#include "Battle/StatusEffects/BattleStatusEffectUtils.h"

UCanStatusEffectBeInflicted * UCanStatusEffectBeInflicted::CanStatusEffectBeInflicted(FStatusHandle StatusEffectID,
                                                                                      const TScriptInterface<IBattler> &Target, FText AlreadyAppliedFormat, FText HasOtherStatusFormat) {
    auto Node = NewObject<UCanStatusEffectBeInflicted>();
    Node->SetWorldContext(Target.GetObject());
    Node->StatusEffectID = StatusEffectID;
    Node->Target = Target;
    Node->AlreadyAppliedFormat = std::move(AlreadyAppliedFormat);
    Node->HasOtherStatusFormat = std::move(HasOtherStatusFormat);
    return Node;
}

UE5Coro::TCoroutine<> UCanStatusEffectBeInflicted::ExecuteCoroutine(FForceLatentCoroutine ForceLatentCoroutine) {
    OnComplete.Broadcast(co_await UBattleStatusEffectUtils::CanStatusEffectBeInflicted(GetWorldContext(), StatusEffectID, Target, AlreadyAppliedFormat, HasOtherStatusFormat));
}