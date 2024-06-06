// "Unreal Pokémon" created by Retro & Chill.

#include "Handlers/PartyMenu/PartyDebugHandler.h"

bool UPartyDebugHandler::ShouldShow_Implementation(const TScriptInterface<IPartyScreen>& Screen, const TScriptInterface<ITrainer>& Trainer, int32 PartyIndex) const {
#if WITH_EDITOR
    return true;
#else
    return false;
#endif
}

void UPartyDebugHandler::Handle_Implementation(const TScriptInterface<IPartyScreen>& Screen, const TScriptInterface<ITrainer>& Trainer, int32 PartyIndex) {
    // TODO: Implement me
}
