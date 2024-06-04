// "Unreal Pokémon" created by Retro & Chill.


#include "Handlers/BagMenu/GiveItemHandler.h"
#include "Bag/Item.h"

bool UGiveItemHandler::ShouldShow_Implementation(const TScriptInterface<IInventoryScreen>& Screen, const FItem &Item, int32 Quantity) const {
    return Item.CanHold();
}

void UGiveItemHandler::Handle_Implementation(const TScriptInterface<IInventoryScreen>& Screen, const FItem &Item, int32 Quantity) {
    // TODO: Implement selecting a Pokémon from the menu
    
}