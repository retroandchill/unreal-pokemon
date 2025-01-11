// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Animations/AnimationObject.h"

UE5Coro::TCoroutine<> UAnimationObject::PlayAnimation(FForceLatentCoroutine Coro) {
    TFuture<void> Future = TFuture<void>(OnBattleAnimationComplete);
    Play();
    co_await Future;
}

void UAnimationObject::AnimationComplete() const {
    OnBattleAnimationComplete->EmplaceResult(0);
}