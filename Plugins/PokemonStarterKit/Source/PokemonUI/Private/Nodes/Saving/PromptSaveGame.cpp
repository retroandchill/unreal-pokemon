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
    auto Layout = UPrimaryGameLayout::GetPrimaryGameLayoutForPrimaryPlayer(WorldContext);
    auto ScreenClass = GetDefault<UPokemonUISettings>()->SaveScreenClass.TryLoadClass<USaveScreen>();
    auto Screen = Layout->PushWidgetToLayerStack<USaveScreen>(RPG::Menus::PrimaryMenuLayerTag, ScreenClass);
    Screen->BindToOnExitSaveScreen(this, [this](bool bSuccess) {
        bSuccess ? Saved.Broadcast() : DidNotSave.Broadcast();
        SetReadyToDestroy();
    });
}