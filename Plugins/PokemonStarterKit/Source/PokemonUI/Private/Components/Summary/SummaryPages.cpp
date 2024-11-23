// "Unreal Pokémon" created by Retro & Chill.

#include "Components/Summary/SummaryPages.h"
#include "Components/Summary/SummaryScreenPage.h"
#include "Components/WidgetSwitcher.h"
#include "Ranges/Algorithm/ForEach.h"
#include "Ranges/Casting/DynamicCast.h"
#include "Ranges/Views/Ints.h"
#include "Ranges/Views/Map.h"

void USummaryPages::OnPokemonSet_Implementation(const TScriptInterface<IPokemon> &NewPokemon) {
    // clang-format off
    UE::Ranges::Ints(0, PageSwitcher->GetNumWidgets()) |
        UE::Ranges::Map(PageSwitcher, &UWidgetSwitcher::GetWidgetAtIndex) |
        UE::Ranges::Map(UE::Ranges::DynamicCastChecked<USummaryScreenPage>) |
        UE::Ranges::ForEach(&UPokemonInfoWidget::SetPokemon, NewPokemon);
    // clang-format on
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