// "Unreal Pokémon" created by Retro & Chill.

#include "Screens/BagScreen.h"
#include "Handlers/BagMenu/BagMenuHandler.h"
#include "Managers/PokemonSubsystem.h"
#include "Utilities/PokemonUIUtils.h"
#include "Windows/CommandWindow.h"
#include "Windows/ItemInfoWindow.h"
#include "Windows/ItemSelectionWindow.h"
#include "Windows/PocketWindow.h"

void UBagScreen::NativeConstruct() {
    Super::NativeConstruct();

    // TODO: Add handler for when you confirm an item
    ItemSelectionWindow->GetOnItemSelected().AddDynamic(this, &UBagScreen::SelectItem);
    ItemSelectionWindow->GetOnCancel().AddDynamic(this, &UBagScreen::CloseScreen);
    ItemSelectionWindow->GetOnItemChanged().AddDynamic(ItemInfoWindow, &UItemInfoWindow::Refresh);
    ItemSelectionWindow->GetOnNoItemSelected().AddDynamic(ItemInfoWindow, &UItemInfoWindow::ClearItem);
    ItemSelectionWindow->GetOnPocketChanged().AddDynamic(PocketWindow, &UPocketWindow::SetCurrentPocket);
    CommandWindow->GetOnCommandSelected().AddDynamic(this, &UBagScreen::UBagScreen::OnItemCommandSelected);
    CommandWindow->GetOnCancel().AddDynamic(this, &UBagScreen::OnItemCommandCanceled);

    auto &Bag = GetGameInstance()->GetSubsystem<UPokemonSubsystem>()->GetBag();
    auto PocketName = UItemHelper::GetPocketNames()[0];
    ItemSelectionWindow->SetBag(Bag, PocketName);
    ItemSelectionWindow->SetActive(true);
    ItemSelectionWindow->SetIndex(0);
}

void UBagScreen::ApplyItemFilter(const FItemFilter &ItemFilter) {
    RefreshScene();
}

FOnItemSelected &UBagScreen::GetOnItemSelected() {
    return OnItemSelected;
}

void UBagScreen::ToggleItemSelection(bool bCanSelect) {
    ItemSelectionWindow->SetActive(bCanSelect);
}

void UBagScreen::RemoveFromStack() {
    CloseScreen();
}

void UBagScreen::RefreshScene() {
    ItemSelectionWindow->RefreshWindow();
}

void UBagScreen::RefreshSelf_Implementation() {
    Super::RefreshSelf_Implementation();
    RefreshScene();
}

void UBagScreen::CreateCommands(const FItem &Item, int32 Quantity) {
    auto Commands = UPokemonUIUtils::CreateCommandListFromHandlers(CommandHandlers, CancelText, this, Item, Quantity);
    CommandWindow->SetCommands(MoveTemp(Commands));
}

void UBagScreen::SelectItem(const FItem &Item, int32 Quantity) {
    if (OnItemSelected.IsBound()) {
        OnItemSelected.Execute(this, Item, Quantity);
        return;
    }

    ToggleItemSelection(false);
    CreateCommands(Item, Quantity);
    CommandWindow->SetIndex(0);
    CommandWindow->SetActive(true);
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
    CommandWindow->SetActive(false);
    CommandWindow->SetVisibility(ESlateVisibility::Hidden);
    ToggleItemSelection(true);
}