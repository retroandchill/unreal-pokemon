// "Unreal Pokémon" created by Retro & Chill.

#include "Screens/BagScreen.h"
#include "Components/Bag/PocketTabWidget.h"
#include "Handlers/BagMenu/BagMenuHandler.h"
#include "Handlers/BagMenu/BagMenuHandlerSet.h"
#include "Managers/PokemonSubsystem.h"
#include "Utilities/PokemonUIUtils.h"
#include "Windows/CommandWindow.h"
#include "Windows/ItemInfoWindow.h"
#include "Windows/ItemSelectionWindow.h"
#include "Windows/PocketWindow.h"
#include "Windows/PokemonSelectionPane.h"

void UBagScreen::NativeConstruct() {
    Super::NativeConstruct();
    
    ItemSelectionWindow->GetOnItemSelected().AddUniqueDynamic(this, &UBagScreen::SelectItem);
    ItemSelectionWindow->GetOnCancel().AddUniqueDynamic(this, &UScreen::CloseScreen);
    ItemSelectionWindow->GetOnItemChanged().AddUniqueDynamic(ItemInfoWindow, &UItemInfoWindow::Refresh);
    ItemSelectionWindow->GetOnNoItemSelected().AddUniqueDynamic(ItemInfoWindow, &UItemInfoWindow::ClearItem);
    ItemSelectionWindow->GetOnPocketChanged().AddUniqueDynamic(PocketWindow, &UPocketWindow::SetCurrentPocket);
    CommandWindow->GetOnCommandSelected().AddUniqueDynamic(this, &UBagScreen::UBagScreen::OnItemCommandSelected);
    CommandWindow->GetOnCancel().AddUniqueDynamic(this, &UBagScreen::OnItemCommandCanceled);
    
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

UItemSelectionWindow * UBagScreen::GetItemSelectionWindow() const {
    return ItemSelectionWindow;
}

void UBagScreen::CreateCommands(const FItem &Item, int32 Quantity) {
    auto Commands =
        UPokemonUIUtils::CreateCommandListFromHandlers(CommandHandlers->GetHandlers(), this, Item, Quantity);
    CommandWindow->SetCommands(MoveTemp(Commands));
}

void UBagScreen::SelectItem(const FItem &Item, int32 Quantity) {
    if (OnItemSelected.IsBound()) {
        OnItemSelected.Execute(this, Item, Quantity);
        OnItemSelected.Unbind();
        return;
    }

    ToggleItemSelection(false);
    CreateCommands(Item, Quantity);
    CommandWindow->SetIndex(0);
    CommandWindow->ActivateWidget();
    CommandWindow->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UBagScreen::OnItemCommandSelected(int32, UCommand *Command) {
    static FName CancelCommand = TEXT("Cancel");
    if (Command->GetID() == CancelCommand) {
        OnItemCommandCanceled();
    } else {
        auto Handler = Command->GetHandler<UBagMenuHandler>();
        check(Handler != nullptr)
        Handler->Handle(this, *ItemSelectionWindow->GetCurrentItem(), ItemSelectionWindow->GetItemQuantity());
    }
}

void UBagScreen::OnItemCommandCanceled() {
    CommandWindow->DeactivateWidget();
    CommandWindow->SetVisibility(ESlateVisibility::Hidden);
    ToggleItemSelection(true);
}