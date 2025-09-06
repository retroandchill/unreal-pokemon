// "Unreal Pokémon" created by Retro & Chill.

#include "Player/ItemSlot.h"
#include "DataManager.h"

const FItem &FItemSlot::GetItem() const
{
    auto ItemData = FDataManager::GetInstance().GetDataTable<FItem>().GetData(Item);
    check(ItemData != nullptr)
    return *ItemData;
}