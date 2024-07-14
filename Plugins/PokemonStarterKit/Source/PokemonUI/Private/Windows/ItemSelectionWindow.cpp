// "Unreal Pokémon" created by Retro & Chill.

#include "Windows/ItemSelectionWindow.h"
#include "Algo/ForEach.h"
#include "Bag/Item.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Bag/ItemOption.h"
#include "DataManager.h"
#include "Memory/CursorMemorySubsystem.h"
#include "Player/Bag.h"
#include <functional>

UItemSelectionWindow::UItemSelectionWindow(const FObjectInitializer &ObjectInitializer)
    : USelectableWidget(ObjectInitializer) {
}

void UItemSelectionWindow::SetBag(const TScriptInterface<IBag> &Bag, FName Pocket) {
    auto &PocketNames = GetGameInstance()->GetSubsystem<UCursorMemorySubsystem>()->GetBagPocketNames();
    CurrentBag = Bag;
    int32 PocketIndex = PocketNames.IndexOfByKey(Pocket);
    PocketIterator = TCircularIterator<FName>(PocketNames, PocketIndex);
    UpdatePocket();
}

const FItem *UItemSelectionWindow::GetCurrentItem() const {
    if (auto Option = GetSelectableOption<UItemOption>(GetIndex()); Option != nullptr) {
        return &Option->GetItem();
    }

    return nullptr;
}

int32 UItemSelectionWindow::GetItemQuantity() const {
    if (auto Option = GetSelectableOption<UItemOption>(GetIndex()); Option != nullptr) {
        return Option->GetQuantity();
    }

    return 0;
}

void UItemSelectionWindow::RefreshWindow() {
    UpdatePocket();
}

void UItemSelectionWindow::ApplyItemFilter(const FItemFilter &ItemFilter) {
    ItemListFilter = ItemFilter;
}

FOnItemChanged &UItemSelectionWindow::GetOnItemSelected() {
    return OnItemSelected;
}

FOnPocketChanged &UItemSelectionWindow::GetOnPocketChanged() {
    return OnPocketChanged;
}

FOnItemChanged &UItemSelectionWindow::GetOnItemChanged() {
    return OnItemChanged;
}

FOnNoItemSelected &UItemSelectionWindow::GetOnNoItemSelected() {
    return OnNoItemSelected;
}

void UItemSelectionWindow::OnSelectionChange_Implementation(int32 OldIndex, int32 NewIndex) {
    Super::OnSelectionChange_Implementation(OldIndex, NewIndex);
    GetGameInstance()->GetSubsystem<UCursorMemorySubsystem>()->UpdatePocketMemory(*PocketIterator, NewIndex);
    if (auto Item = GetCurrentItem(); Item != nullptr) {
        OnItemChanged.Broadcast(*Item, GetSelectableOption<UItemOption>(NewIndex)->GetQuantity());
    } else {
        OnNoItemSelected.Broadcast();
    }
}

void UItemSelectionWindow::ProcessConfirm_Implementation(int32 CurrentIndex) {
    UItemOption *Option = GetSelectableOption<UItemOption>(CurrentIndex);
    check(Option != nullptr)
    OnItemSelected.Broadcast(Option->GetItem(), Option->GetQuantity());
}

void UItemSelectionWindow::UpdatePocket() {
    ClearSelectableOptions();
    CurrentBag->ForEachInPocket(*PocketIterator, std::bind_front(&UItemSelectionWindow::AddItemToWindow, this));
    SetIndex(GetGameInstance()->GetSubsystem<UCursorMemorySubsystem>()->GetBagPocketMemory()[*PocketIterator]);
    OnPocketChanged.Broadcast(*PocketIterator);
}

void UItemSelectionWindow::AddItemToWindow(FName ItemName, int32 Quantity) {
    auto Item = FDataManager::GetInstance().GetDataTable<FItem>().GetData(ItemName);
    check(Item != nullptr)
    if (ItemListFilter.IsBound() && !ItemListFilter.Execute(*Item)) {
        return;
    }

    auto Option = WidgetTree->ConstructWidget(ItemEntryClass);
    Option->SetItem(ItemName, Quantity);
    SlotOption(Option, GetItemCount());
}