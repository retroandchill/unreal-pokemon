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

void UItemOption::NativeOnCurrentTextStyleChanged() {
    Super::NativeOnCurrentTextStyleChanged();
    /*
    auto TextStyle = GetCurrentTextStyleClass();
    if (ItemNameText != nullptr) {
        // ItemNameText->SetTextStyle(TextStyle);
    }
    if (ItemQuantityText != nullptr) {
        // ItemQuantityText->SetTextStyle(TextStyle);
    }
    */
}

void UItemOption::SetItem(FName Item, int32 Quantity) {
    CurrentItem = Item;
    QuantityHeld = Quantity;

    /*
    ItemNameText->SetText(ItemData.RealName);

    if (ItemData.ShouldShowQuantity()) {
        ItemQuantityText->SetText(FText::Format(FText::FromStringView(TEXT("×{0}")), {Quantity}));
    } else {
        ItemQuantityText->SetText(FText::FromStringView(TEXT("")));
    }
    */
    
    OnItemSet(GetItem(), Quantity);
}