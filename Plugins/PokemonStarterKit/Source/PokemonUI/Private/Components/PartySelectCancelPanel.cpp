// "Unreal Pokémon" created by Retro & Chill.

#include "Components/PartySelectCancelPanel.h"
#include "Windows/SelectableWidget.h"

void UPartySelectCancelPanel::SetOwner(USelectableWidget *NewOwner) {
    Owner = NewOwner;
}

bool UPartySelectCancelPanel::IsPanelSelected() const {
    return IsHovered();
}

void UPartySelectCancelPanel::Refresh() {
    RefreshVisuals();
}
