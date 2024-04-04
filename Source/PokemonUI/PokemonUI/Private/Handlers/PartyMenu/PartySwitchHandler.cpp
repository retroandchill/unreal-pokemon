// "Unreal Pokémon" created by Retro & Chill.


#include "Handlers/PartyMenu/PartySwitchHandler.h"

#include "Screens/PartyScreen.h"

bool UPartySwitchHandler::ShouldShow(const IPartyScreen& Screen, const TArray<TScriptInterface<IPokemon>>& Party,
                                     int32 PartyIndex) const {
	return Party.Num() > 1;
}

void UPartySwitchHandler::Handle(IPartyScreen& Screen, TArray<TScriptInterface<IPokemon>>& Party, int32 PartyIndex) {
	Screen.BeginSwitch(PartyIndex);
}
