// "Unreal Pokémon" created by Retro & Chill.

#include "Components/Bag/ItemOption.h"
#include "Bag/Item.h"
#include "DataManager.h"
#include "Primatives/DisplayText.h"

const FItem &UItemOption::GetItem() const {
    auto ItemData = FDataManager::GetInstance().GetDataTable<FItem>().GetData(CurrentItem);
    check(ItemData != nullptr)
    return *ItemData;
}

void UItemOption::SetItem_Implementation(FName Item, int32 Quantity) {
    CurrentItem = Item;

    auto &ItemData = GetItem();
    ItemNameText->SetText(ItemData.RealName);

    if (ItemData.ShouldShowQuantity()) {
        ItemQuantityText->SetText(FText::Format(FText::FromStringView(TEXT("×{0}")), {Quantity}));
    } else {
        ItemQuantityText->SetText(FText::FromStringView(TEXT("")));
    }
}