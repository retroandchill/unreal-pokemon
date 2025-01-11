// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Animations/AnimationActor.h"

UE5Coro::TCoroutine<> AAnimationActor::PlayAnimation(FForceLatentCoroutine Coro) {
    TFuture<void> Future = TFuture<void>(OnBattleAnimationComplete);
    Play();
    co_await Future;
    Destroy();
}

void AAnimationActor::AnimationComplete() {
    OnBattleAnimationComplete->EmplaceResult(0);
}