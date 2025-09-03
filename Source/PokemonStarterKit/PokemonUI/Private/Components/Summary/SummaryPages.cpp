// "Unreal Pokémon" created by Retro & Chill.

#include "Components/Summary/SummaryPages.h"
#include "Components/Summary/SummaryScreenPage.h"
#include "Components/WidgetSwitcher.h"
#include "RetroLib/Casting/DynamicCast.h"
#include "RetroLib/Ranges/Algorithm/NameAliases.h"

void USummaryPages::OnPokemonSet_Implementation(const TScriptInterface<IPokemon> &NewPokemon) {
    // clang-format off
    Retro::Ranges::Views::Iota(0, PageSwitcher->GetNumWidgets()) |
        Retro::Ranges::Views::Transform(Retro::BindMethod<&UWidgetSwitcher::GetWidgetAtIndex>(PageSwitcher)) |
        Retro::Ranges::Views::Transform(Retro::DynamicCastChecked<USummaryScreenPage>) |
        Retro::Ranges::ForEach(Retro::BindBack<&UPokemonInfoWidget::SetPokemon>(NewPokemon));
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