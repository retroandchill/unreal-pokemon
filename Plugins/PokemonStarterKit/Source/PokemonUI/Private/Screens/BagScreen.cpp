// "Unreal Pokémon" created by Retro & Chill.

#include "Screens/BagScreen.h"
#include "Components/Bag/ItemSelectionWindow.h"
#include "Components/Bag/PocketTabWidget.h"
#include "Managers/PokemonSubsystem.h"

void UBagScreen::NativeConstruct() {
    Super::NativeConstruct();

    auto &Bag = GetGameInstance()->GetSubsystem<UPokemonSubsystem>()->GetBag();
    ItemSelectionWindow->SetBag(Bag);
    PocketTabWidget->SetItemSelectionWindow(ItemSelectionWindow);
    ItemSelectionWindow->ActivateWidget();
}

void UBagScreen::ApplyItemFilter(const FItemFilter &ItemFilter) {
    RefreshScene();
}

FOnItemSelected &UBagScreen::GetOnItemSelected() {
    return OnItemSelected;
}

void UBagScreen::ToggleItemSelection(bool bCanSelect) {
    if (bCanSelect) {
        ItemSelectionWindow->ActivateWidget();
    } else {
        ItemSelectionWindow->DeactivateWidget();
    }
}

void UBagScreen::RemoveFromStack() {
    CloseScreen();
}

void UBagScreen::CloseScreen() {
    Super::CloseScreen();
    OnItemSelected.Unbind();
}

void UBagScreen::RefreshScene() {
    ItemSelectionWindow->RefreshWindow();
}

void UBagScreen::RefreshSelf_Implementation() {
    Super::RefreshSelf_Implementation();
    RefreshScene();
}

UItemSelectionWindow *UBagScreen::GetItemSelectionWindow() const {
    return ItemSelectionWindow;
}

UItemInfoWindow *UBagScreen::GetItemInfoWindow() const {
    return ItemInfoWindow;
}

UPocketTabWidget *UBagScreen::GetPocketTabWidget() const {
    return PocketTabWidget;
}

UPocketWindow *UBagScreen::GetPocketWindow() const {
    return PocketWindow;
}

void UBagScreen::SelectItem(const FItem &Item, int32 Quantity) {
    if (OnItemSelected.IsBound()) {
        OnItemSelected.Execute(this, Item, Quantity);
        OnItemSelected.Unbind();
        return;
    }

    ToggleItemSelection(false);
    ShowItemCommands();
}