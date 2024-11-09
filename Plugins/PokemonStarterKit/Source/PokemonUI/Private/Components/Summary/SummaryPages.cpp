// "Unreal Pokémon" created by Retro & Chill.

#include "Components/Summary/SummaryPages.h"
#include "Components/Summary/SummaryScreenPage.h"
#include "Components/WidgetSwitcher.h"
#include "Ranges/Views/Enumerate.h"
#include "Ranges/Views/Ints.h"
#include "Ranges/Algorithm/ForEach.h"

void USummaryPages::NativeConstruct() {
    Super::NativeConstruct();
    CastChecked<USummaryScreenPage>(PageSwitcher->GetActiveWidget())->OnPageShown();
}

void USummaryPages::OnPokemonSet_Implementation(const TScriptInterface<IPokemon> &NewPokemon) {
    // clang-format off
    UE::Ranges::Ints(PageSwitcher->GetNumWidgets()) |
        UE::Ranges::Map(PageSwitcher, &UWidgetSwitcher::GetWidgetAtIndex) |
        UE::Ranges::CastType<USummaryScreenPage> |
        UE::Ranges::ForEach(&UPokemonInfoWidget::SetPokemon, NewPokemon);
    // clang-format on
    if (OnPokemonChange.IsBound()) {
        OnPokemonChange.Execute(GetPokemon());
    }
}

FOnPokemonChange &USummaryPages::GetOnPokemonChange() {
    return OnPokemonChange;
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