// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Transitions/BattleTransitionActor.h"

FDelegateHandle ABattleTransitionActor::BindToOnComplete(FOnBattleTransitionComplete::FDelegate &&Binding) {
    return OnBattleTransitionComplete.Add(std::move(Binding));
}

void ABattleTransitionActor::CompleteTransition() {
    OnBattleTransitionComplete.Broadcast();
    Destroy();
}