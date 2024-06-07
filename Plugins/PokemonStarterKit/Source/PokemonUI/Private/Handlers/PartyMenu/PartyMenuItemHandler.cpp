// "Unreal Pokémon" created by Retro & Chill.

#include "Handlers/PartyMenu/PartyMenuItemHandler.h"
#include "Pokemon/Pokemon.h"
#include "Screens/PartyScreen.h"
#include "Trainers/Trainer.h"

bool UPartyMenuItemHandler::ShouldShow_Implementation(const TScriptInterface<IPartyScreen> &Screen,
                                                      const TScriptInterface<ITrainer> &Trainer,
                                                      int32 PartyIndex) const {
    auto HoldItem = Trainer->GetPokemon(PartyIndex)->GetHoldItem();
    return HoldItem == nullptr || !HoldItem->IsMail();
}

void UPartyMenuItemHandler::Handle_Implementation(const TScriptInterface<IPartyScreen> &Screen,
                                                  const TScriptInterface<ITrainer> &Trainer, int32 PartyIndex) {
    Screen->SetCommandHelpText(HelpText);
    Screen->ShowCommands(SubCommands);
}
