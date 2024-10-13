// "Unreal Pokémon" created by Retro & Chill.

#include "Nodes/ChooseItemFromBag.h"
#include "PokemonUISettings.h"
#include "PrimaryGameLayout.h"
#include "Screens/BagScreen.h"

UChooseItemFromBag *UChooseItemFromBag::ChooseItemFromBag(const UObject *WorldContextObject,
                                                          const FItemFilter &ItemFilter) {
    auto Node = NewObject<UChooseItemFromBag>();
    Node->WorldContextObject = WorldContextObject;
    Node->ItemFilter = ItemFilter;
    return Node;
}

void UChooseItemFromBag::Activate() {
    auto Screen = UBagScreen::AddBagScreenToStack(WorldContextObject);
    Screen->ApplyItemFilter(ItemFilter);
    Screen->GetOnItemSelected().BindUObject(this, &UChooseItemFromBag::ExecuteOnSelected);
    Screen->GetOnScreenClosed().AddUniqueDynamic(this, &UChooseItemFromBag::ExecuteOnCanceled);
}

void UChooseItemFromBag::ExecuteOnSelected(const TScriptInterface<IInventoryScreen> &Screen, const FItem &Item,
                                           int32 Quantity) {
    OnSelected.Broadcast(Screen, Item, Quantity);
    SetReadyToDestroy();
}

void UChooseItemFromBag::ExecuteOnCanceled() {
    OnCanceled.Broadcast();
    SetReadyToDestroy();
}