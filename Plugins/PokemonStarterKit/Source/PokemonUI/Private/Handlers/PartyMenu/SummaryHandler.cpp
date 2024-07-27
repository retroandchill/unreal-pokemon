// "Unreal Pokémon" created by Retro & Chill.

#include "Handlers/PartyMenu/SummaryHandler.h"
#include "PrimaryGameLayout.h"
#include "Screens/PartyScreen.h"
#include "Screens/PokemonSummaryScreen.h"
#include "Trainers/Trainer.h"

void USummaryHandler::Handle_Implementation(const TScriptInterface<IPartyScreen> &Screen,
                                            const TScriptInterface<ITrainer> &Trainer, int32 PartyIndex) {
    check(SummaryScreenClass != nullptr)
    auto Overlay = UPrimaryGameLayout::GetPrimaryGameLayout(Screen->GetPlayerController());
    auto NewScreen =
        Overlay->PushWidgetToLayerStack<UPokemonSummaryScreen>(RPG::Menus::PrimaryMenuLayerTag, SummaryScreenClass.LoadSynchronous());
    NewScreen->SetInitialPokemon(Trainer->GetParty(), PartyIndex);
}
