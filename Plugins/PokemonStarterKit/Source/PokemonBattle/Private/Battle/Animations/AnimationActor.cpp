// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Animations/AnimationActor.h"

void AAnimationActor::BindDelegateToAnimationComplete_Implementation(const FBattleAnimationCompleteCallback &Delegate) {
    OnBattleAnimationComplete.Add(Delegate);
}

void AAnimationActor::RemoveDelegateFromAnimationComplete_Implementation(
    const FBattleAnimationCompleteCallback &Delegate) {
    OnBattleAnimationComplete.Remove(Delegate);
}

void AAnimationActor::AnimationComplete() {
    OnBattleAnimationComplete.Broadcast();
    Destroy();
}