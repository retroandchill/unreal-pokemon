// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Animations/BattleAnimation.h"

// Add default functionality here for any IBattleAnimation functions that are not pure virtual.
UE5Coro::TCoroutine<> IBattleAnimation::PlayAnimation(const TScriptInterface<IBattleAnimation> &Animation)
{
    if (Animation == nullptr)
    {
        co_return;
    }

    co_await Animation->PlayAnimation();
}