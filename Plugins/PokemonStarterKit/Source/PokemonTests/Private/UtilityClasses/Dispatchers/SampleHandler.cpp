// "Unreal Pokémon" created by Retro & Chill.

#include "SampleHandler.h"
#include "Bag/Item.h"

void USampleHandler::Handle_Implementation(const TScriptInterface<IInventoryScreen> &Screen, const FItem &Item,
                                           int32 Quantity) {
    Super::Handle_Implementation(Screen, Item, Quantity);
    ItemID = Item.ID;
    ItemQuantity = Quantity;
}