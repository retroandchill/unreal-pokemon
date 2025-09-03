// "Unreal Pokémon" created by Retro & Chill.

#include "Components/BattleItemSelectionOption.h"
#include "Bag/Item.h"
#include "DataManager.h"

void UBattleItemSelectionOption::SetItem(FName Item, int32 Quantity) {
    ItemID = Item;
    ItemQuantity = Quantity;
    static auto &ItemTable = FDataManager::GetInstance().GetDataTable<FItem>();
    OnItemSet(ItemTable.GetDataChecked(ItemID), Quantity);
}