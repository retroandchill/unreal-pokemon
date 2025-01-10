// "Unreal Pokémon" created by Retro & Chill.

#include "Nodes/Saving/PromptSaveGame.h"
#include "Screens/SaveScreen.h"

UPromptSaveGame *UPromptSaveGame::PromptToSave(const UObject *WorldContextObject) {
    auto Node = NewObject<UPromptSaveGame>();
    Node->SetWorldContext(WorldContextObject);
    return Node;
}

UE5Coro::TCoroutine<> UPromptSaveGame::ExecuteCoroutine(FForceLatentCoroutine Coro) {
    auto Screen = USaveScreen::AddSaveScreenToStack(GetWorldContext());
    auto bSuccess = co_await Screen->UntilSaveComplete();
    bSuccess ? Saved.Broadcast() : DidNotSave.Broadcast();
}