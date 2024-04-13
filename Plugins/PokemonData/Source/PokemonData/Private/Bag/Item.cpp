// "Unreal Pokémon" created by Retro & Chill.
#include "Bag/Item.h"
#include "DataManager.h"

FItem::FItem() = default;

TArray<FName> UItemHelper::GetItemNames() {
    return FDataManager::GetInstance().GetDataTable<FItem>().GetTableRowNames();
}
