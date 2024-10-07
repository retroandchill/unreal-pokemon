// "Unreal Pokémon" created by Retro & Chill.

#include "Components/Bag/ItemInfoWindow.h"
#include "Bag/Item.h"
#include "Components/DisplayText.h"
#include "Components/Image.h"
#include "Graphics/AssetClasses.h"
#include "Utilities/WidgetUtilities.h"

void UItemInfoWindow::Refresh_Implementation(const FItem &Item, int32 Quantity) {
    auto ItemGraphic = Pokemon::Assets::Graphics::ItemIcons.LoadAssetAsync(Item.ID).GetValue().LoadSynchronous();
    ItemIcon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    UWidgetUtilities::SetBrushFromAsset(ItemIcon, ItemGraphic, true);
    ItemDescription->SetText(Item.Description);
}
void UItemInfoWindow::ClearItem_Implementation() {
    ItemIcon->SetVisibility(ESlateVisibility::Hidden);
    ItemDescription->SetText(FText::GetEmpty());
}