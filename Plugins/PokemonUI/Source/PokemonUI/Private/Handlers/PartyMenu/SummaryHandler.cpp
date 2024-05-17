// "Unreal Pokémon" created by Retro & Chill.

#include "Handlers/PartyMenu/SummaryHandler.h"
#include "RPGPlayerController.h"
#include "Screens/PartyScreen.h"
#include "Screens/PokemonSummaryScreen.h"
#include "Trainers/Trainer.h"

void USummaryHandler::Handle(IPartyScreen &Screen, const ITrainer &Trainer, int32 PartyIndex) {
    check(SummaryScreenClass != nullptr)
    auto NewScreen = Screen.GetPlayerController().AddScreenToStack(SummaryScreenClass);
    NewScreen->SetInitialPokemon(Trainer.GetParty(), PartyIndex);
}
