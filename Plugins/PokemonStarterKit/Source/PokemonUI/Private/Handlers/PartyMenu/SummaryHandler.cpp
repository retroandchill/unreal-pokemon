// "Unreal Pokémon" created by Retro & Chill.

#include "Handlers/PartyMenu/SummaryHandler.h"
#include "RPGMenusSubsystem.h"
#include "Screens/PartyScreen.h"
#include "Screens/PokemonSummaryScreen.h"
#include "Trainers/Trainer.h"

void USummaryHandler::Handle_Implementation(const TScriptInterface<IPartyScreen> &Screen,
                                            const TScriptInterface<ITrainer> &Trainer, int32 PartyIndex) {
    check(SummaryScreenClass != nullptr)
    auto Subsystem = Screen->GetPlayerController()->GetLocalPlayer()->GetSubsystem<URPGMenusSubsystem>();
    auto NewScreen = Subsystem->AddScreenToStack(SummaryScreenClass);
    NewScreen->SetInitialPokemon(Trainer->GetParty(), PartyIndex);
}
