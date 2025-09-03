// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Animations/AnimationObject.h"

UE5Coro::TCoroutine<> UAnimationObject::PlayAnimation() {
    Play();
    co_await OnBattleAnimationComplete.GetFuture();
}

void UAnimationObject::AnimationComplete() {
    OnBattleAnimationComplete.EmplaceValue(0);
}