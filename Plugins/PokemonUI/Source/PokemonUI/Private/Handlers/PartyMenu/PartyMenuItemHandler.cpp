// "Unreal Pokémon" created by Retro & Chill.

#include "Handlers/PartyMenu/PartyMenuItemHandler.h"

bool UPartyMenuItemHandler::ShouldShow_Implementation(const TScriptInterface<IPartyScreen>& Screen, const TScriptInterface<ITrainer>& Trainer, int32 PartyIndex) const {
    // TODO: Implement me
    return Super::ShouldShow_Implementation(Screen, Trainer, PartyIndex);
}

void UPartyMenuItemHandler::Handle_Implementation(const TScriptInterface<IPartyScreen>& Screen, const TScriptInterface<ITrainer>& Trainer, int32 PartyIndex) {
    // TODO: Implement me
}
