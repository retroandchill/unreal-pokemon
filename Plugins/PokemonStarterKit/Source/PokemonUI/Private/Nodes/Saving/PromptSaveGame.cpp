// "Unreal Pokémon" created by Retro & Chill.

#include "Nodes/Saving/PromptSaveGame.h"
#include "PokemonUISettings.h"
#include "PrimaryGameLayout.h"
#include "Screens/SaveScreen.h"

UPromptSaveGame *UPromptSaveGame::PromptToSave(const UObject *WorldContext) {
    auto Node = NewObject<UPromptSaveGame>();
    Node->WorldContext = WorldContext;
    return Node;
}

UE5Coro::TCoroutine<> UPromptSaveGame::ExecuteCoroutine(FForceLatentCoroutine Coro) {
    auto Screen = USaveScreen::AddSaveScreenToStack(WorldContext);
    auto bSuccess = co_await Screen->UntilSaveComplete();
    bSuccess ? Saved.Broadcast() : DidNotSave.Broadcast();
}