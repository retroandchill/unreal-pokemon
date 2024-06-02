// "Unreal Pokémon" created by Retro & Chill.

#include "Windows/ItemSelectionWindow.h"
#include "Algo/ForEach.h"
#include "Bag/Item.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Bag/ItemOption.h"
#include "Player/Bag.h"
#include <functional>

UItemSelectionWindow::UItemSelectionWindow(const FObjectInitializer &ObjectInitializer) : USelectableWidget(ObjectInitializer) {
    for (auto Pocket : PocketNames) {
        PocketMemory.Add(Pocket, 0);
    }
}

void UItemSelectionWindow::SetBag(const TScriptInterface<IBag> &Bag, FName Pocket) {
    CurrentBag = Bag;
    int32 PocketIndex = PocketNames.IndexOfByKey(Pocket);
    PocketIterator = TCircularIterator<FName>(PocketNames, PocketIndex);
    UpdatePocket();
}

const FItem *UItemSelectionWindow::GetCurrentItem() const {
    if (Options.IsValidIndex(GetIndex())) {
        return &Options[GetIndex()]->GetItem();
    }

    return nullptr;
}

int32 UItemSelectionWindow::GetItemCount_Implementation() const {
    return Options.Num();
}

FOnPocketChanged & UItemSelectionWindow::GetOnPocketChanged() {
    return OnPocketChanged;
}

FOnItemChanged & UItemSelectionWindow::GetOnItemChanged() {
    return OnItemChanged;
}

void UItemSelectionWindow::OnSelectionChange_Implementation(int32 OldIndex, int32 NewIndex) {
    Super::OnSelectionChange_Implementation(OldIndex, NewIndex);
    PocketMemory[*PocketIterator] = NewIndex;
    if (auto Item = GetCurrentItem(); Item != nullptr) {
        OnItemChanged.Broadcast(*Item, Options[NewIndex]->GetQuantity());
    }
}

void UItemSelectionWindow::ReceiveMoveCursor(ECursorDirection Direction) {
    Super::ReceiveMoveCursor(Direction);
    if (!PocketIterator.CanCycle()) {
        return;
    }

    using enum ECursorDirection;
    bool bPocketChanged = false;
    if (Direction == Left) {
        --PocketIterator;
        bPocketChanged = true;
    } else if (Direction == Right) {
        ++PocketIterator;
        bPocketChanged = true;
    }

    if (bPocketChanged) {
        UpdatePocket();
    }
}

void UItemSelectionWindow::UpdatePocket() {
    Algo::ForEach(Options, &UWidget::RemoveFromParent);
    Options.Empty();
    CurrentBag->ForEachInPocket(*PocketIterator, std::bind_front(&UItemSelectionWindow::AddItemToWindow, this));
    SetIndex(PocketMemory[*PocketIterator]);
    OnPocketChanged.Broadcast(*PocketIterator);
}

void UItemSelectionWindow::AddItemToWindow(FName ItemName, int32 Quantity) {
    auto Option = WidgetTree->ConstructWidget(ItemEntryClass);
    Option->SetItem(ItemName, Quantity);
    SlotItem(Option, Options.Num());
    Options.Emplace(Option);
}