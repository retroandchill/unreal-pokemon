// "Unreal Pokémon" created by Retro & Chill.

#include "Handlers/PartyMenu/PartySwitchHandler.h"
#include "Screens/PartyScreen.h"
#include "Trainers/Trainer.h"

bool UPartySwitchHandler::ShouldShow_Implementation(const TScriptInterface<IPartyScreen>& Screen, const TScriptInterface<ITrainer>& Trainer, int32 PartyIndex) const {
    return Trainer->GetParty().Num() > 1;
}

void UPartySwitchHandler::Handle_Implementation(const TScriptInterface<IPartyScreen>& Screen, const TScriptInterface<ITrainer>& Trainer, int32 PartyIndex) {
    Screen->BeginSwitch(PartyIndex);
}
