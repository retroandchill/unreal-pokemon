// "Unreal Pokémon" created by Retro & Chill.

#include "ScreenTransitions/FadeScreenOut.h"
#include "GridUtils.h"

UFadeScreenOut *UFadeScreenOut::FadeScreenOut(const UObject *WorldContextObject)
{
    auto Node = NewObject<UFadeScreenOut>();
    Node->SetWorldContext(WorldContextObject);
    return Node;
}

UE5Coro::TCoroutine<> UFadeScreenOut::ExecuteCoroutine(FForceLatentCoroutine ForceLatentCoroutine)
{
    auto GameMode = UGridUtils::GetGridBasedGameMode(GetWorldContext());
    check(GameMode != nullptr)

    co_await GameMode->FadeOut();
    OnScreenTransitionFinished.Broadcast();
}
