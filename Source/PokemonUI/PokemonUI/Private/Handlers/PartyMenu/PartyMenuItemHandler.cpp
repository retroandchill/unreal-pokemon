// "Unreal Pokémon" created by Retro & Chill.


#include "Handlers/PartyMenu/PartyMenuItemHandler.h"

bool UPartyMenuItemHandler::ShouldShow(const IPartyScreen& Screen, const TArray<TSharedRef<IPokemon>>& Party,
	int32 PartyIndex) const {
	// TODO: Implement me
	return Super::ShouldShow(Screen, Party, PartyIndex);
}

void UPartyMenuItemHandler::Handle(IPartyScreen& Screen, TArray<TSharedRef<IPokemon>>& Party, int32 PartyIndex) {
	// TODO: Implement me
}
