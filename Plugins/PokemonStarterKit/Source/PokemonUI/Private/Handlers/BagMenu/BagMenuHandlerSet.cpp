// "Unreal Pokémon" created by Retro & Chill.

#include "Handlers/BagMenu/BagMenuHandlerSet.h"

const TArray<TObjectPtr<UBagMenuHandler>> &UBagMenuHandlerSet::GetHandlers() const {
    return Handlers;
}