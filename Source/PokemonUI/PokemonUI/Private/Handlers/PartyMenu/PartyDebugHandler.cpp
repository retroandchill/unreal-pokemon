// "Unreal Pokémon" created by Retro & Chill.


#include "Handlers/PartyMenu/PartyDebugHandler.h"

bool UPartyDebugHandler::ShouldShow(const IPartyScreen& Screen, const TArray<TSharedRef<IPokemon>>& Party,
	int32 PartyIndex) const {
#if WITH_EDITOR
	return true;
#else
	return false;
#endif
}

void UPartyDebugHandler::Handle(IPartyScreen& Screen, TArray<TSharedRef<IPokemon>>& Party, int32 PartyIndex) {
	// TODO: Implement me
}
