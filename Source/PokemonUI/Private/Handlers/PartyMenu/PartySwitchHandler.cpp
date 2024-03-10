// "Unreal Pokémon" created by Retro & Chill.


#include "Handlers/PartyMenu/PartySwitchHandler.h"

#include "Screens/PartyScreen.h"

bool UPartySwitchHandler::ShouldShow(const IPartyScreen& Screen, const TArray<TSharedRef<IPokemon>>& Party,
                                     int32 PartyIndex) const {
	return Party.Num() > 0;
}

void UPartySwitchHandler::Handle(IPartyScreen& Screen, TArray<TSharedRef<IPokemon>>& Party, int32 PartyIndex) {
	Screen.BeginSwitch(PartyIndex);
}
