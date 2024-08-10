// "Unreal Pokémon" created by Retro & Chill.


#include "Components/BattleItemSelectionOption.h"
#include "DataManager.h"
#include "Bag/Item.h"
#include "Components/DisplayText.h"

void UBattleItemSelectionOption::SetItem(FName Item, int32 Quantity) {
    ItemID = Item;
    ItemQuantity = Quantity;

    static auto &ItemTable = FDataManager::GetInstance().GetDataTable<FItem>();
    auto &ItemData = ItemTable.GetDataChecked(ItemID);

    ItemQuantityText->SetText(ItemData.RealName);
    ItemQuantityText->SetText(FText::FromString(FString::FromInt(ItemQuantity)));
}