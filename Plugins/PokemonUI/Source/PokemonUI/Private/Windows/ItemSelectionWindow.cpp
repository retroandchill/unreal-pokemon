// "Unreal Pokémon" created by Retro & Chill.

#include "Windows/ItemSelectionWindow.h"
#include "Algo/ForEach.h"
#include "Bag/Item.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Bag/ItemOption.h"
#include "Player/Bag.h"
#include <functional>

void UItemSelectionWindow::SetBag(const TScriptInterface<IBag> &Bag, FName Pocket) {
    CurrentBag = Bag;
    int32 PocketIndex = PocketNames.IndexOfByKey(Pocket);
    PocketIterator = TCircularIterator<FName>(PocketNames, PocketIndex);
    Algo::ForEach(Options, &UWidget::RemoveFromParent);
    Options.Empty();
    Bag->ForEachInPocket(*PocketIterator, std::bind_front(&UItemSelectionWindow::AddItemToWindow, this));
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

void UItemSelectionWindow::AddItemToWindow(FName ItemName, int32 Quantity) {
    auto Option = WidgetTree->ConstructWidget(ItemEntryClass);
    Option->SetItem(ItemName, Quantity);
    SlotItem(Option, Options.Num());
    Options.Emplace(Option);
}