﻿// "Unreal Pokémon" created by Retro & Chill.

#include "Player/DefaultBag.h"
#include "Bag/Item.h"
#include "DataManager.h"
#include "Player/ItemSlot.h"
#include "Player/Sorting/BagSorter.h"
#include "PokemonDataSettings.h"
#include "RetroLib/Optionals/OrElseValue.h"
#include "RetroLib/Optionals/Transform.h"
#include "RetroLib/Ranges/Algorithm/NameAliases.h"
#include <functional>

/**
 * Check if the given item slot is for the given item ID.
 * @param ItemID The item ID to validate
 * @param Slot The slot to check against
 * @return Is this slot for the given item
 */
bool ItemSlotMatches(FName ItemID, const FItemSlot &Slot) {
    return Slot.Item == ItemID;
}

void UDefaultBag::Initialize(const FBagDTO &DTO) {
    ItemSlots = DTO.ItemSlots;
}

FBagDTO UDefaultBag::ToDTO() const {
    return {.ItemSlots = ItemSlots};
}

int32 UDefaultBag::GetItemQuantity(FItemHandle ItemID) const {
    auto Pocket = GetPocket(ItemID);
    if (Pocket == nullptr) {
        return 0;
    }

    auto ItemSlot = Pocket->FindByPredicate(std::bind_front(&ItemSlotMatches, ItemID));
    return ItemSlot != nullptr ? ItemSlot->Quantity : 0;
}

bool UDefaultBag::HasItemWithTag(FName Tag) const {
    auto &ItemTable = FDataManager::GetInstance().GetDataTable<FItem>();
    // clang-format off
    return ItemSlots |
                 Retro::Ranges::Views::Values |
                 Retro::Ranges::Views::Transform(&FPocket::Items) |
                 Retro::Ranges::Views::Join |
                 Retro::Ranges::Views::Transform(&FItemSlot::Item) |
                 Retro::Ranges::Views::Transform(Retro::BindFront<&TDataTableProxy<FItem>::GetDataChecked>(std::ref(ItemTable))) |
                 Retro::Ranges::Views::Transform(&FItem::Tags) |
                 Retro::Ranges::AnyOf(Retro::BindBack<&TArray<FName>::Contains<FName>>(Tag));
    // clang-format on
}

bool UDefaultBag::CanObtainItem(FItemHandle ItemID) const {
    auto Settings = GetDefault<UPokemonDataSettings>();
    if (auto ItemQuantity = GetItemQuantity(ItemID); ItemQuantity > 0) {
        return ItemQuantity < Settings->MaxItemsPerSlot;
    }

    auto Item = FDataManager::GetInstance().GetDataTable<FItem>().GetData(ItemID);
    check(Item != nullptr)
    auto Pocket = ItemSlots.Find(Item->Pocket);
    if (Pocket == nullptr) {
        return false;
    }

    const auto &[DisplayName, MaxPocketSize, bAutoSort] = Settings->PocketInfo.FindChecked(Item->Pocket);
    // clang-format off
    return MaxPocketSize |
           Retro::Optionals::Transform([&Pocket](int32 Max) {
               return Pocket->Items.Num() < Max;
           }) |
           Retro::Optionals::OrElseValue(true);
    // clang-format on
}

int32 UDefaultBag::ObtainItem(FItemHandle ItemID, int32 Amount) {
    auto Settings = GetDefault<UPokemonDataSettings>();
    auto &Pocket = GetPocket(ItemID);
    int32 SlotMax = Settings->MaxItemsPerSlot;
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

int32 UDefaultBag::RemoveItem(FItemHandle ItemID, int32 Amount) {
    auto Settings = GetDefault<UPokemonDataSettings>();
    auto &Pocket = GetPocket(ItemID);
    auto SlotIndex = Pocket.IndexOfByPredicate(std::bind_front(&ItemSlotMatches, ItemID));
    if (SlotIndex == INDEX_NONE) {
        return 0;
    }

    int32 &Quantity = Pocket[SlotIndex].Quantity;
    int32 QuantityBefore = Quantity;
    Quantity = FMath::Clamp(Quantity - Amount, 0, Settings->MaxItemsPerSlot);
    int32 Change = QuantityBefore - Quantity;

    if (Quantity == 0) {
        Pocket.RemoveAt(SlotIndex);
    }

    return Change;
}

void UDefaultBag::ClearBag() {
    ItemSlots.Empty();
}

void UDefaultBag::SortPocket(FName Pocket, const IBagSorter &Sorter) {
    Sorter.SortPocket(ItemSlots.FindOrAdd(Pocket).Items);
}

void UDefaultBag::ForEachInPocket(FName Pocket, const TFunctionRef<void(FName, int32)> &Callback) const {
    auto Items = ItemSlots.Find(Pocket);
    if (Items == nullptr) {
        return;
    }

    for (auto &[Item, Quantity] : Items->Items) {
        Callback(Item, Quantity);
    }
}

void UDefaultBag::ForEachItem(const TFunctionRef<void(FName, int32)> &Callback) const {
    for (auto &[Pocket, Items] : ItemSlots) {
        for (auto &[Item, Quantity] : Items.Items) {
            Callback(Item, Quantity);
        }
    }
}

TArray<FItemSlot> &UDefaultBag::GetPocket(FName ItemID) {
    auto Item = FDataManager::GetInstance().GetDataTable<FItem>().GetData(ItemID);
    check(Item != nullptr)
    return ItemSlots.FindOrAdd(Item->Pocket).Items;
}

const TArray<FItemSlot> *UDefaultBag::GetPocket(FName ItemID) const {
    auto Item = FDataManager::GetInstance().GetDataTable<FItem>().GetData(ItemID);
    check(Item != nullptr)
    auto Pocket = ItemSlots.Find(Item->Pocket);
    return Pocket != nullptr ? &Pocket->Items : nullptr;
}