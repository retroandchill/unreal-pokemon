// "Unreal Pokémon" created by Retro & Chill.

#include "Components/SummaryScreenPage.h"

void USummaryScreenPage::RefreshInfo_Implementation(const TScriptInterface<IPokemon> &Pokemon) {
    // No implementation by default
}

bool USummaryScreenPage::CanSelectOnPage_Implementation() const {
    return false;
}

FPageSelected &USummaryScreenPage::GetPageSelected() {
    return PageSelected;
}