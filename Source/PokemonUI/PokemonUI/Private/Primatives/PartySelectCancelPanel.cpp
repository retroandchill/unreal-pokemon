// "Unreal Pokémon" created by Retro & Chill.

#include "Primatives/PartySelectCancelPanel.h"

#include "Windows/SelectableWidget.h"

void UPartySelectCancelPanel::SetOwner(USelectableWidget *NewOwner) { Owner = NewOwner; }

void UPartySelectCancelPanel::SetMenuIndex(int32 Index) {
    MenuIndex = Index;
    Refresh();
}

bool UPartySelectCancelPanel::IsPanelSelected() const { return Owner != nullptr && Owner->GetIndex() == MenuIndex; }

void UPartySelectCancelPanel::Refresh() { RefreshVisuals(); }
