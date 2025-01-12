// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Animations/AnimationObject.h"

UE5Coro::TCoroutine<> UAnimationObject::PlayAnimation(FForceLatentCoroutine) {
    Play();
    co_await TFuture<void>(OnBattleAnimationComplete);
}

void UAnimationObject::AnimationComplete() const {
    OnBattleAnimationComplete->EmplaceResult(0);
}