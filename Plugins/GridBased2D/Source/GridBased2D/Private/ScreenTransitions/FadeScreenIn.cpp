// "Unreal Pokémon" created by Retro & Chill.

#include "ScreenTransitions/FadeScreenIn.h"

#include "GridUtils.h"

UFadeScreenIn *UFadeScreenIn::FadeScreenIn(const UObject *WorldContext) {
    auto Node = NewObject<UFadeScreenIn>();
    Node->WorldContext = WorldContext;
    return Node;
}

void UFadeScreenIn::Activate() {
    auto GameMode = UGridUtils::GetGridBasedGameMode(WorldContext);
    check(GameMode != nullptr)

    FScreenTransitionCallback Callback;
    Callback.BindDynamic(this, &UFadeScreenIn::TransitionFinished);
    GameMode->FadeIn(Callback);
}

void UFadeScreenIn::TransitionFinished() {
    OnScreenTransitionFinished.Broadcast();
}
