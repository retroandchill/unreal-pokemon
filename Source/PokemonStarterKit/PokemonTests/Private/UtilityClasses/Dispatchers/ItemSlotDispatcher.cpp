// "Unreal Pokémon" created by Retro & Chill.

#include "ItemSlotDispatcher.h"
#include "Bag/Item.h"

void UItemSlotDispatcher::ReceiveItem(const FItem &Item, int32 ItemQuantity) {
    ItemID = Item.ID;
    Quantity = ItemQuantity;
}