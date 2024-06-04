// "Unreal Pokémon" created by Retro & Chill.


#include "Handlers/BagMenu/BagMenuHandler.h"

bool UBagMenuHandler::ShouldShow_Implementation(const TScriptInterface<IInventoryScreen> &Screen, const FItem &Item,
    int32 Quantity) const {
    return true;
}

void UBagMenuHandler::Handle_Implementation(const TScriptInterface<IInventoryScreen> &Screen, const FItem &Item,
    int32 Quantity) {
    // No default action needed
}