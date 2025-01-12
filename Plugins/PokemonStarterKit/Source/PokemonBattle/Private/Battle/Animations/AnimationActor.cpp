// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Animations/AnimationActor.h"

UE5Coro::TCoroutine<> AAnimationActor::PlayAnimation() {
    Play();
    co_await TFuture<void>(OnBattleAnimationComplete);
    Destroy();
}

void AAnimationActor::AnimationComplete() {
    OnBattleAnimationComplete->EmplaceResult(0);
}