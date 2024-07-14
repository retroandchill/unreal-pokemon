// "Unreal Pokémon" created by Retro & Chill.

#include "Components/Summary/HoldItemInfo.h"
#include "CommonTextBlock.h"
#include "Components/Image.h"
#include "Graphics/GraphicsLoadingSubsystem.h"
#include "Pokemon/Pokemon.h"
#include "Utilities/WidgetUtilities.h"

void UHoldItemInfo::Refresh_Implementation(const TScriptInterface<IPokemon> &Pokemon) {
    Super::Refresh_Implementation(Pokemon);

    using enum ESlateVisibility;
    if (auto HoldItem = Pokemon->GetHoldItem(); HoldItem != nullptr) {
        ItemNameText->SetText(HoldItem->RealName);
        auto Item = GetGameInstance()->GetSubsystem<UGraphicsLoadingSubsystem>()->GetItemIcon(HoldItem->ID);
        UWidgetUtilities::SetBrushFromAsset(ItemIcon, Item, true);
        ItemIcon->SetVisibility(SelfHitTestInvisible);
    } else {
        ItemNameText->SetText(NoItemText);
        ItemIcon->SetVisibility(Hidden);
    }

    ShinyIcon->SetVisibility(Pokemon->IsShiny() ? SelfHitTestInvisible : Hidden);
}