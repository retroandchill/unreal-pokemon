// "Unreal Pokémon" created by Retro & Chill.

#include "Components/Bag/ItemInfoWindow.h"
#include "Bag/Item.h"
#include "Components/DisplayText.h"
#include "Components/Image.h"
#include "Graphics/AssetClasses.h"
#include "Ranges/Optional/GetPtrOrNull.h"
#include "Utilities/WidgetUtilities.h"

void UItemInfoWindow::Refresh_Implementation(const FItem &Item, int32 Quantity) {
    // clang-format off
    auto ItemGraphic = Pokemon::Assets::Graphics::ItemIcons.LoadAsset(Item.ID) |
                       UE::Optionals::Map([](const FImageAsset &Asset) -> auto &{ return Asset.Get(); }) |
                       UE::Optionals::GetPtrOrNull;
    // clang-format on
    ItemIcon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    UWidgetUtilities::SetBrushFromAsset(ItemIcon, ItemGraphic, true);
    ItemDescription->SetText(Item.Description);
}

void UItemInfoWindow::ClearItem_Implementation() {
    ItemIcon->SetVisibility(ESlateVisibility::Hidden);
    ItemDescription->SetText(FText::GetEmpty());
}