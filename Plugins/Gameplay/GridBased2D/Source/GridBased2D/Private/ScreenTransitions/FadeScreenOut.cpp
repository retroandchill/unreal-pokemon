// "Unreal Pokémon" created by Retro & Chill.

#include "ScreenTransitions/FadeScreenOut.h"
#include "GridUtils.h"

UFadeScreenOut *UFadeScreenOut::FadeScreenOut(const UObject *WorldContextObject)
{
    auto *Node = NewObject<UFadeScreenOut>();
    Node->WorldContext = WorldContextObject;
    return Node;
}

void UFadeScreenOut::Activate()
{
    auto *GameMode = UGridUtils::GetGridBasedGameMode(WorldContext);
    check(GameMode != nullptr);

    GameMode->BindToOnScreenTransitionFinished(FSimpleDelegate::CreateWeakLambda(this, [this] {
        OnScreenTransitionFinished.Broadcast();
        SetReadyToDestroy();
    }));
    GameMode->ScreenFadeOut();
}
