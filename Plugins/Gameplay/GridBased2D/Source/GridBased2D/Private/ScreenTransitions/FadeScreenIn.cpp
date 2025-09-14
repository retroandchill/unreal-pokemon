// "Unreal Pokémon" created by Retro & Chill.

#include "ScreenTransitions/FadeScreenIn.h"
#include "GridUtils.h"

UFadeScreenIn *UFadeScreenIn::FadeScreenIn(const UObject *WorldContextObject)
{
    auto Node = NewObject<UFadeScreenIn>();
    Node->SetWorldContext(WorldContextObject);
    return Node;
}

UE5Coro::TCoroutine<> UFadeScreenIn::ExecuteCoroutine(FForceLatentCoroutine ForceLatentCoroutine)
{
    auto GameMode = UGridUtils::GetGridBasedGameMode(GetWorldContext());
    check(GameMode != nullptr)

    co_await GameMode->FadeIn();
    OnScreenTransitionFinished.Broadcast();
}
