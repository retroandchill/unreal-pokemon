// "Unreal Pokémon" created by Retro & Chill.


#include "Windows/SummaryPages.h"
#include "Components/WidgetSwitcher.h"

UWidgetSwitcher* USummaryPages::GetPageSwitcher() const {
    return PageSwitcher;
}

int32 USummaryPages::GetItemCount_Implementation() const {
    return PageSwitcher->GetNumWidgets();
}

int32 USummaryPages::GetColumnCount_Implementation() const {
    return PageSwitcher->GetNumWidgets();
}

void USummaryPages::OnSelectionChange_Implementation(int32 OldIndex, int32 NewIndex) {
    Super::OnSelectionChange_Implementation(OldIndex, NewIndex);
    PageSwitcher->SetActiveWidgetIndex(NewIndex);
}