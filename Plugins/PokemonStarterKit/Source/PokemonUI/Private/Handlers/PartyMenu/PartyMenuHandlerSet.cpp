// "Unreal Pokémon" created by Retro & Chill.


#include "Handlers/PartyMenu/PartyMenuHandlerSet.h"
#include "Handlers/PartyMenu/PartyMenuHandler.h"

const TArray<TObjectPtr<UPartyMenuHandler>> & UPartyMenuHandlerSet::GetHandlers() const {
    return Handlers;
}