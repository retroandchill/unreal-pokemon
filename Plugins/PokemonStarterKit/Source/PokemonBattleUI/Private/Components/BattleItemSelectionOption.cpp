// "Unreal Pokémon" created by Retro & Chill.


#include "Components/BattleItemSelectionOption.h"
#include "DataManager.h"
#include "Bag/Item.h"
#include "Components/DisplayText.h"
#include "Graphics/GraphicsLoadingSubsystem.h"
#include "Utilities/WidgetUtilities.h"

void UBattleItemSelectionOption::SetItem(FName Item, int32 Quantity) {
    ItemID = Item;
    ItemQuantity = Quantity;

    static auto &ItemTable = FDataManager::GetInstance().GetDataTable<FItem>();
    auto &ItemData = ItemTable.GetDataChecked(ItemID);

    auto Subsystem = GetGameInstance()->GetSubsystem<UGraphicsLoadingSubsystem>();
    auto IconAsset = Subsystem->GetItemIcon(ItemID);
    UWidgetUtilities::SetBrushFromAsset(ItemIcon, IconAsset, true);
    
    ItemNameText->SetText(ItemData.RealName);
    if (ItemData.ShouldShowQuantity()) {
        ItemQuantityText->SetText(FText::Format(FText::FromStringView(TEXT("×{0}")), {Quantity}));
    } else {
        ItemQuantityText->SetText(FText::FromStringView(TEXT("")));
    }
}