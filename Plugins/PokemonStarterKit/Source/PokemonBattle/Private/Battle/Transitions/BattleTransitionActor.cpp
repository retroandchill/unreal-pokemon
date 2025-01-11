// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Transitions/BattleTransitionActor.h"

UE5Coro::TCoroutine<> ABattleTransitionActor::Execute() {
    TransitionToBattle();
    co_await TFuture<void>(OnBattleTransitionComplete);
    Destroy();
}

void ABattleTransitionActor::CompleteTransition() {
    OnBattleTransitionComplete->EmplaceResult(0);
}