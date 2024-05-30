// "Unreal Pokémon" created by Retro & Chill.


#include "Player/DefaultBag.h"
#include "DataManager.h"
#include "Bag/Item.h"
#include "Player/ItemSlot.h"
#include "Player/Sorting/BagSorter.h"
#include "Settings/BagSettings.h"

#include <functional>

/**
 * Check if the given item slot is for the given item ID.
 * @param ItemID The item ID to validate
 * @param Slot The slot to check against
 * @return Is this slot for the given item
 */
bool ItemSlotMatches(FName ItemID, const FItemSlot& Slot) {
    return Slot.Item == ItemID;
}

void UDefaultBag::Initialize() {
    // No initialization needed
}

int32 UDefaultBag::GetItemQuantity(FName ItemID) const {
    auto Pocket = GetPocket(ItemID);
    if (Pocket == nullptr) {
        return 0;
    }
    
    auto ItemSlot = Pocket->FindByPredicate(std::bind_front(&ItemSlotMatches, ItemID));
    return ItemSlot != nullptr ? ItemSlot->Quantity : 0;
}

int32 UDefaultBag::ObtainItem(FName ItemID, int32 Amount) {
    auto &Pocket = GetPocket(ItemID);
    int32 SlotMax = GetDefault<UBagSettings>()->GetMaxItemsPerSlot();
    auto ItemSlot = Pocket.FindByPredicate(std::bind_front(&ItemSlotMatches, ItemID));
    int32 QuantityBefore;
    if (ItemSlot == nullptr) {
        ItemSlot = &Pocket.Emplace_GetRef(ItemID, FMath::Clamp(Amount, 0, SlotMax));
        QuantityBefore = 0;
    } else {
        QuantityBefore = ItemSlot->Quantity;
        ItemSlot->Quantity = FMath::Clamp(ItemSlot->Quantity + Amount, 0, SlotMax);
    }

    return ItemSlot->Quantity - QuantityBefore;
}

int32 UDefaultBag::RemoveItem(FName ItemID, int32 Amount) {
    auto &Pocket = GetPocket(ItemID);
    int32 SlotMax = GetDefault<UBagSettings>()->GetMaxItemsPerSlot();
    auto SlotIndex = Pocket.IndexOfByPredicate(std::bind_front(&ItemSlotMatches, ItemID));
    if (SlotIndex == INDEX_NONE) {
        return 0;
    }

    int32 &Quantity = Pocket[SlotIndex].Quantity;
    int32 QuantityBefore = Quantity;
    Quantity = FMath::Clamp(Quantity - Amount, 0, SlotMax);
    int32 Change = QuantityBefore - Quantity;

    if (Quantity == 0) {
        Pocket.RemoveAt(SlotIndex);
    }

    return Change;
}

void UDefaultBag::SortPocket(uint8 Pocket, const IBagSorter &Sorter) {
    Sorter.SortPocket(ItemSlots.FindOrAdd(Pocket).Items);
}

void UDefaultBag::ForEachInPocket(uint8 Pocket, TFunctionRef<void(FName, int32)> Callback) const {
    auto Items = ItemSlots.Find(Pocket);
    if (Items == nullptr) {
            return;
    }

    for (auto&[Item, Quantity] : Items->Items) {
        Callback(Item, Quantity);
    }
}

TArray<FItemSlot> & UDefaultBag::GetPocket(FName ItemID) {
    auto Item = FDataManager::GetInstance().GetDataTable<FItem>().GetData(ItemID);
    check(Item != nullptr)
    return ItemSlots.FindOrAdd(Item->Pocket).Items;
}

const TArray<FItemSlot> * UDefaultBag::GetPocket(FName ItemID) const {
    auto Item = FDataManager::GetInstance().GetDataTable<FItem>().GetData(ItemID);
    check(Item != nullptr)
    auto Pocket = ItemSlots.Find(Item->Pocket);
    return Pocket != nullptr ? &Pocket->Items : nullptr;
}