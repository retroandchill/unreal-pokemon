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

void UPromptSaveGame::Activate() {
    auto Screen = USaveScreen::AddSaveScreenToStack(WorldContext);
    Screen->BindToOnExitSaveScreen(this, [this](bool bSuccess) {
        bSuccess ? Saved.Broadcast() : DidNotSave.Broadcast();
        SetReadyToDestroy();
    });
}