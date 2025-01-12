// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Transitions/BattleTransitionActor.h"

UE5Coro::TCoroutine<> ABattleTransitionActor::Execute() {
    TransitionToBattle();
    co_await TFuture<void>(OnBattleTransitionComplete);
    Destroy();
}

UE5Coro::TCoroutine<> ABattleTransitionActor::Execute(ABattleTransitionActor *Transition) {
    if (IsValid(Transition)) {
        co_await Transition->Execute();
    }
}

void ABattleTransitionActor::CompleteTransition() {
    OnBattleTransitionComplete->EmplaceResult(0);
}