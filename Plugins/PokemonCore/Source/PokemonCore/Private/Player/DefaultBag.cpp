// "Unreal Pokémon" created by Retro & Chill.


#include "Player/DefaultBag.h"
#include "Player/ItemSlot.h"
#include "Settings/BagSettings.h"

/**
 * Check if the given item slot is for the given item ID.
 * @param Slot The slot to check against
 * @param ItemID The item ID to validate
 * @return Is this slot for the given item
 */
bool ItemSlotMatches(const FItemSlot& Slot, FName ItemID) {
    return Slot.Item == ItemID;
}

int32 UDefaultBag::GetItemQuantity(FName ItemID) const {
    auto ItemSlot = ItemSlots.FindByPredicate(&ItemSlotMatches);
    return ItemSlot != nullptr ? ItemSlot->Quantity : 0;
}

int32 UDefaultBag::ObtainItem(FName ItemID, int32 Amount) {
    int32 SlotMax = GetDefault<UBagSettings>()->GetMaxItemsPerSlot();
    auto ItemSlot = ItemSlots.FindByPredicate(&ItemSlotMatches);
    int32 QuantityBefore;
    if (ItemSlot == nullptr) {
        ItemSlot = &ItemSlots.Emplace_GetRef(ItemID, FMath::Clamp(Amount, 0, SlotMax));
        QuantityBefore = 0;
    } else {
        QuantityBefore = ItemSlot->Quantity;
        ItemSlot->Quantity = FMath::Clamp(ItemSlot->Quantity + Amount, 0, SlotMax);
    }

    return ItemSlot->Quantity - QuantityBefore;
}

int32 UDefaultBag::RemoveItem(FName ItemID, int32 Amount) {
    int32 SlotMax = GetDefault<UBagSettings>()->GetMaxItemsPerSlot();
    auto SlotIndex = ItemSlots.IndexOfByPredicate(&ItemSlotMatches);
    if (SlotIndex == INDEX_NONE) {
        return 0;
    }

    int32 &Quantity = ItemSlots[SlotIndex].Quantity;
    int32 QuantityBefore = Quantity;
    Quantity = FMath::Clamp(Quantity - Amount, 0, SlotMax);
    int32 Change = QuantityBefore - Quantity;

    if (Quantity == 0) {
        ItemSlots.RemoveAt(SlotIndex);
    }

    return Change;
}