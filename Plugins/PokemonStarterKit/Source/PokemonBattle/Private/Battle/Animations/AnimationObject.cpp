// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Animations/AnimationObject.h"

void UAnimationObject::BindDelegateToAnimationComplete_Implementation(
    const FBattleAnimationCompleteCallback &Delegate) {
    OnBattleAnimationComplete.Add(Delegate);
}

void UAnimationObject::RemoveDelegateFromAnimationComplete_Implementation(
    const FBattleAnimationCompleteCallback &Delegate) {
    OnBattleAnimationComplete.Remove(Delegate);
}

void UAnimationObject::AnimationComplete() const {
    OnBattleAnimationComplete.Broadcast();
}