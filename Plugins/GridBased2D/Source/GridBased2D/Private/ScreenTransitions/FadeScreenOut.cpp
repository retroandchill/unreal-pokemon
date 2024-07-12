// "Unreal Pokémon" created by Retro & Chill.

#include "ScreenTransitions/FadeScreenOut.h"
#include "GridUtils.h"

UFadeScreenOut *UFadeScreenOut::FadeScreenOut(const UObject *WorldContext) {
    auto Node = NewObject<UFadeScreenOut>();
    Node->WorldContext = WorldContext;
    return Node;
}

void UFadeScreenOut::Activate() {
    auto GameMode = UGridUtils::GetGridBasedGameMode(WorldContext);
    check(GameMode != nullptr)

    FScreenTransitionCallback Callback;
    Callback.BindDynamic(this, &UFadeScreenOut::TransitionFinished);
    GameMode->FadeIn(Callback);
}

void UFadeScreenOut::TransitionFinished() {
    OnScreenTransitionFinished.Broadcast();
    SetReadyToDestroy();
}
