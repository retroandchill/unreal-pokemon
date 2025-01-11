// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Animations/PlayBattleAnimation.h"

UPlayBattleAnimation *UPlayBattleAnimation::PlayBattleAnimation(const TScriptInterface<IBattleAnimation> &Animation) {
    auto Node = NewObject<UPlayBattleAnimation>();
    Node->SetWorldContext(Animation.GetObject());
    Node->Animation = Animation;
    return Node;
}

UE5Coro::TCoroutine<> UPlayBattleAnimation::ExecuteCoroutine(FForceLatentCoroutine Coro) {
    co_await Animation->PlayAnimation();
    AnimationComplete.Broadcast();
}