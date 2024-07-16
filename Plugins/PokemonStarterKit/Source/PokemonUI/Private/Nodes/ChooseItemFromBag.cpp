// "Unreal Pokémon" created by Retro & Chill.

#include "Nodes/ChooseItemFromBag.h"
#include "PrimaryGameLayout.h"
#include "Screens/BagScreen.h"

UChooseItemFromBag *UChooseItemFromBag::ChooseItemFromBag(const UObject *WorldContextObject,
                                                          TSubclassOf<UBagScreen> ScreenClass,
                                                          const FItemFilter &ItemFilter) {
    auto Node = NewObject<UChooseItemFromBag>();
    Node->WorldContextObject = WorldContextObject;
    Node->ScreenClass = ScreenClass;
    Node->ItemFilter = ItemFilter;
    return Node;
}

void UChooseItemFromBag::Activate() {
    auto Layout = UPrimaryGameLayout::GetPrimaryGameLayoutForPrimaryPlayer(WorldContextObject);
    auto Screen = Layout->PushWidgetToLayerStack<UBagScreen>(RPG::Menus::PrimaryMenuLayerTag, ScreenClass);
    Screen->ApplyItemFilter(ItemFilter);
    Screen->GetOnItemSelected().BindUObject(this, &UChooseItemFromBag::ExecuteOnSelected);
    Screen->GetOnScreenClosed().AddUniqueDynamic(this, &UChooseItemFromBag::ExecuteOnCanceled);
}

void UChooseItemFromBag::ExecuteOnSelected(const TScriptInterface<IInventoryScreen> &Screen, const FItem &Item,
                                           int32 Quantity) const {
    OnSelected.Broadcast(Screen, Item, Quantity);
}

void UChooseItemFromBag::ExecuteOnCanceled() {
    OnCanceled.Broadcast();
}