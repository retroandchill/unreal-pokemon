// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Animations/AnimationActor.h"

UE5Coro::TCoroutine<> AAnimationActor::PlayAnimation() {
    Play();
    co_await OnBattleAnimationComplete.GetFuture();
    Destroy();
}

void AAnimationActor::AnimationComplete() {
    OnBattleAnimationComplete.EmplaceValue(0);
}