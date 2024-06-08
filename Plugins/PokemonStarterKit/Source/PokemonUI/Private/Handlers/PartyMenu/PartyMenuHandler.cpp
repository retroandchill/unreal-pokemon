// "Unreal Pokémon" created by Retro & Chill.

#include "Handlers/PartyMenu/PartyMenuHandler.h"

bool UPartyMenuHandler::ShouldShow_Implementation(const TScriptInterface<IPartyScreen> &Screen,
                                                  const TScriptInterface<ITrainer> &Trainer, int32 PartyIndex) const {
    return true;
}

void UPartyMenuHandler::Handle_Implementation(const TScriptInterface<IPartyScreen> &Screen,
                                              const TScriptInterface<ITrainer> &Trainer, int32 PartyIndex) {
    // No effect by default, must implement to utilize
}
