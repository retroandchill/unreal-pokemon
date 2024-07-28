// "Unreal Pokémon" created by Retro & Chill.

#include "Components/Summary/SummaryScreenPage.h"

void USummaryScreenPage::RefreshInfo_Implementation(const TScriptInterface<IPokemon> &Pokemon) {
    // No implementation by default
}

void USummaryScreenPage::OnPageShown() {
    // No implementation by default
}

void USummaryScreenPage::OnPageHidden() {
    // No implementation by default
}

TSubclassOf<UCommonButtonStyle> USummaryScreenPage::GetTabButtonStyle() const {
    return TabButtonStyle;
}