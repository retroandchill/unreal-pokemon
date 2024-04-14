// "Unreal Pokémon" created by Retro & Chill.

#include "Handlers/PartyMenu/PartySwitchHandler.h"
#include "Screens/PartyScreen.h"
#include "Trainers/Trainer.h"

bool UPartySwitchHandler::ShouldShow(const IPartyScreen &Screen, const ITrainer &Trainer, int32 PartyIndex) const {
    return Trainer.GetParty().Num() > 1;
}

void UPartySwitchHandler::Handle(IPartyScreen &Screen, const ITrainer &Trainer, int32 PartyIndex) {
    Screen.BeginSwitch(PartyIndex);
}
