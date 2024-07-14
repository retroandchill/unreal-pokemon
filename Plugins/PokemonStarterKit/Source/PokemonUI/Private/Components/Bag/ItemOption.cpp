// "Unreal Pokémon" created by Retro & Chill.

#include "Components/Bag/ItemOption.h"
#include "CommonTextBlock.h"
#include "Bag/Item.h"
#include "DataManager.h"

const FItem &UItemOption::GetItem() const {
    auto ItemData = FDataManager::GetInstance().GetDataTable<FItem>().GetData(CurrentItem);
    check(ItemData != nullptr)
    return *ItemData;
}

int32 UItemOption::GetQuantity() const {
    return QuantityHeld;
}

void UItemOption::NativeOnCurrentTextStyleChanged() {
    Super::NativeOnCurrentTextStyleChanged();
    auto TextStyle = GetCurrentTextStyleClass();
    ItemNameText->SetStyle(TextStyle);
    ItemQuantityText->SetStyle(TextStyle);
}

void UItemOption::SetItem_Implementation(FName Item, int32 Quantity) {
    CurrentItem = Item;
    QuantityHeld = Quantity;

    auto &ItemData = GetItem();
    ItemNameText->SetText(ItemData.RealName);

    if (ItemData.ShouldShowQuantity()) {
        ItemQuantityText->SetText(FText::Format(FText::FromStringView(TEXT("×{0}")), {Quantity}));
    } else {
        ItemQuantityText->SetText(FText::FromStringView(TEXT("")));
    }
}