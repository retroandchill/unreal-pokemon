// "Unreal Pokémon" created by Retro & Chill.

#include "Components/Summary/SummaryPages.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Summary/SummaryScreenPage.h"
#include "Input/CommonUIInputTypes.h"

void USummaryPages::NativeConstruct() {
    Super::NativeConstruct();

    auto CreateBindArgs = [this](UInputAction *Action, auto Function, bool bDisplayInActionBar = false) {
        FBindUIActionArgs BindArgs(Action, FSimpleDelegate::CreateUObject(this, Function));
        BindArgs.bDisplayInActionBar = bDisplayInActionBar;
        return BindArgs;
    };

    InspectActionHandler = RegisterUIActionBinding(CreateBindArgs(SelectionAction, &USummaryPages::Select));
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
    PageSwitcher->SetActiveWidgetIndex(PageIndex);
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

void USummaryPages::Select() const {
    OnSelected.Broadcast(PageSwitcher->GetActiveWidgetIndex());
}

void USummaryPages::NextPage() {
    PageSwitcher->SetActiveWidgetIndex(
        FMath::Min(PageSwitcher->GetActiveWidgetIndex() + 1, PageSwitcher->GetNumWidgets() - 1));
}

void USummaryPages::PreviousPage() {
    PageSwitcher->SetActiveWidgetIndex(FMath::Max(PageSwitcher->GetActiveWidgetIndex() - 1, 0));
}

FOnSelected &USummaryPages::GetOnSelected() {
    return OnSelected;
}

UWidgetSwitcher *USummaryPages::GetPageSwitcher() const {
    return PageSwitcher;
}