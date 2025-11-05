// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Transitions/BattleTransitionActor.h"

UE5Coro::TCoroutine<> ABattleTransitionActor::Execute() {
    TransitionToBattle();
    co_await OnBattleTransitionComplete.GetFuture();
    Destroy();
}

UE5Coro::TCoroutine<> ABattleTransitionActor::Execute(ABattleTransitionActor *Transition) {
    if (IsValid(Transition)) {
        co_await Transition->Execute();
    }
}

void ABattleTransitionActor::CompleteTransition() {
    OnBattleTransitionComplete.EmplaceValue(0);
}