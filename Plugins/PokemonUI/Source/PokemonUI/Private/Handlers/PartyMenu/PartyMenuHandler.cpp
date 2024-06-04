// "Unreal Pokémon" created by Retro & Chill.

#include "Handlers/PartyMenu/PartyMenuHandler.h"

bool UPartyMenuHandler::ShouldShow(const IPartyScreen &Screen, const ITrainer &Trainer, int32 PartyIndex) const {
    return true;
}

void UPartyMenuHandler::Handle(IPartyScreen &Screen, const ITrainer &Trainer, int32 PartyIndex) {
    // No effect by default, must implement to utilize
}
