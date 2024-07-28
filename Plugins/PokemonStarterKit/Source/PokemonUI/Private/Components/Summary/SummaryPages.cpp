// "Unreal Pokémon" created by Retro & Chill.

#include "Components/Summary/SummaryPages.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Summary/SummaryScreenPage.h"

void USummaryPages::NativeConstruct() {
    Super::NativeConstruct();
    CastChecked<USummaryScreenPage>(PageSwitcher->GetActiveWidget())->OnPageShown();
}

void USummaryPages::SetPokemon(const TScriptInterface<IPokemon>& NewPokemon) {
    CurrentPokemon = NewPokemon;
    Refresh(CurrentPokemon);
    if (OnPokemonChange.IsBound()) {
        OnPokemonChange.Execute(CurrentPokemon);
    }
}

FOnPokemonChange &USummaryPages::GetOnPokemonChange() {
    return OnPokemonChange;
}

const TScriptInterface<IPokemon> &USummaryPages::GetCurrentPokemon() const {
    return CurrentPokemon;
}

void USummaryPages::SetPage(int32 PageIndex) {
    CastChecked<USummaryScreenPage>(PageSwitcher->GetActiveWidget())->OnPageHidden();
    PageSwitcher->SetActiveWidgetIndex(PageIndex);
    CastChecked<USummaryScreenPage>(PageSwitcher->GetActiveWidget())->OnPageShown();
}

TArray<USummaryScreenPage *> USummaryPages::GetPages() const {
    TArray<USummaryScreenPage *> Pages;
    for (int32 i = 0; i < PageSwitcher->GetNumWidgets(); i++) {
        Pages.Add(CastChecked<USummaryScreenPage>(PageSwitcher->GetWidgetAtIndex(i)));
    }
    return Pages;
}

int32 USummaryPages::GetCurrentPageIndex() const {
    return PageSwitcher->GetActiveWidgetIndex();
}

void USummaryPages::NextPage() {
    SetPage(FMath::Min(PageSwitcher->GetActiveWidgetIndex() + 1, PageSwitcher->GetNumWidgets() - 1));
}

void USummaryPages::PreviousPage() {
    SetPage(FMath::Max(PageSwitcher->GetActiveWidgetIndex() - 1, 0));
}

UWidgetSwitcher *USummaryPages::GetPageSwitcher() const {
    return PageSwitcher;
}