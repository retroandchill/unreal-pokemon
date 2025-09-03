// "Unreal Pokémon" created by Retro & Chill.

#include "Components/Bag/ItemOption.h"
#include "Bag/Item.h"
#include "CommonTextBlock.h"
#include "DataManager.h"

const FItem &UItemOption::GetItem() const {
    auto ItemData = FDataManager::GetInstance().GetDataTable<FItem>().GetData(CurrentItem);
    check(ItemData != nullptr)
    return *ItemData;
}

int32 UItemOption::GetQuantity() const {
    return QuantityHeld;
}

void UItemOption::SetItem(FName Item, int32 Quantity) {
    CurrentItem = Item;
    QuantityHeld = Quantity;
    OnItemSet(GetItem(), Quantity);
}