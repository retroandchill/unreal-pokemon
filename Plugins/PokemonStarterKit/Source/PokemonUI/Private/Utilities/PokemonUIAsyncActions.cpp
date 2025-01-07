// "Unreal Pokémon" created by Retro & Chill.


#include "Utilities/PokemonUIAsyncActions.h"
#include "PrimaryGameLayout.h"
#include "Screens/TextDisplayScreen.h"

FVoidCoroutine UPokemonUIAsyncActions::DisplayMessage(const UObject *WorldContextObject, const FText &Message,
    const FLatentActionInfo &) {
    if (auto Controller = WorldContextObject->GetWorld()->GetFirstPlayerController(); Controller == nullptr) {
        co_return;
    }

    auto Layout = UPrimaryGameLayout::GetPrimaryGameLayoutForPrimaryPlayer(WorldContextObject);
    auto Screen = Cast<UTextDisplayScreen>(Layout->GetLayerWidget(RPG::Menus::OverlayMenuLayerTag)->GetActiveWidget());
    if (Screen == nullptr) {
        Screen = UTextDisplayScreen::AddTextDisplayScreenToOverlay(WorldContextObject);
    }
    Screen->SetText(Message);
    co_await Screen->NextMessage;
}