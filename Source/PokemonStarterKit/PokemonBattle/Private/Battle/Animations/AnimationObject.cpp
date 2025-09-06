// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Animations/AnimationObject.h"

UE5Coro::TCoroutine<> UAnimationObject::PlayAnimation()
{
    OnBattleAnimationComplete = MakeUnique<TPromise<int32>>();
    Play();
    co_await OnBattleAnimationComplete->GetFuture();
}

void UAnimationObject::AnimationComplete()
{
    OnBattleAnimationComplete->EmplaceValue(0);
}