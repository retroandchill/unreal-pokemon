// "Unreal Pokémon" created by Retro & Chill.

#include "ScreenTransitions/FadeScreenIn.h"
#include "GridUtils.h"

UFadeScreenIn *UFadeScreenIn::FadeScreenIn(const UObject *WorldContextObject)
{
    auto *Node = NewObject<UFadeScreenIn>();
    Node->WorldContext = WorldContextObject;
    return Node;
}

void UFadeScreenIn::Activate()
{
    auto *GameMode = UGridUtils::GetGridBasedGameMode(WorldContext);
    check(GameMode != nullptr);

    GameMode->BindToOnScreenTransitionFinished(FSimpleDelegate::CreateWeakLambda(this, [this] {
        OnScreenTransitionFinished.Broadcast();
        SetReadyToDestroy();
    }));
    GameMode->ScreenFadeIn();   
}
