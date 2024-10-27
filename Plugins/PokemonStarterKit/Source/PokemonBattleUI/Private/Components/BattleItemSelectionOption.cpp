﻿// "Unreal Pokémon" created by Retro & Chill.

#include "Components/BattleItemSelectionOption.h"
#include "Bag/Item.h"
#include "Components/DisplayText.h"
#include "DataManager.h"
#include "Graphics/AssetClasses.h"
#include "Ranges/Optional/GetPtrOrNull.h"
#include "Utilities/WidgetUtilities.h"

void UBattleItemSelectionOption::SetItem(FName Item, int32 Quantity) {
    ItemID = Item;
    ItemQuantity = Quantity;

    static auto &ItemTable = FDataManager::GetInstance().GetDataTable<FItem>();
    auto &ItemData = ItemTable.GetDataChecked(ItemID);

    // clang-format off
    auto IconAsset = Pokemon::Assets::Graphics::ItemIcons.LoadAsset(ItemID) |
                       UE::Optionals::Map([](const FImageAsset &Asset) -> auto &{ return Asset.Get(); }) |
                       UE::Optionals::GetPtrOrNull;
    // clang-format on
    UWidgetUtilities::SetBrushFromAsset(ItemIcon, IconAsset, true);

    ItemNameText->SetText(ItemData.RealName);
    if (ItemData.ShouldShowQuantity()) {
        ItemQuantityText->SetText(FText::Format(FText::FromStringView(TEXT("×{0}")), {Quantity}));
    } else {
        ItemQuantityText->SetText(FText::FromStringView(TEXT("")));
    }
}