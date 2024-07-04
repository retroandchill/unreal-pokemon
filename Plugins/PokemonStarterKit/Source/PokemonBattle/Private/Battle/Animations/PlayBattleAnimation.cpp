// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/Animations/PlayBattleAnimation.h"

UPlayBattleAnimation * UPlayBattleAnimation::PlayBattleAnimation(const TScriptInterface<IBattleAnimation> &Animation) {
    auto Node = NewObject<UPlayBattleAnimation>();
    Node->Animation = Animation;
    return Node;
}

void UPlayBattleAnimation::Activate() {
    AnimationDelegate = MakeUnique<FBattleAnimationCompleteCallback>();
    AnimationDelegate->BindDynamic(this, &UPlayBattleAnimation::OnAnimationComplete);
    IBattleAnimation::Execute_BindDelegateToAnimationComplete(Animation.GetObject(), *AnimationDelegate);
    IBattleAnimation::Execute_Play(Animation.GetObject());
}

void UPlayBattleAnimation::OnAnimationComplete() {
    AnimationComplete.Broadcast();
    IBattleAnimation::Execute_RemoveDelegateFromAnimationComplete(Animation.GetObject(), *AnimationDelegate);
    AnimationDelegate.Reset();
}