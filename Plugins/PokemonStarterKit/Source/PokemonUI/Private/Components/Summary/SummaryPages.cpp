// "Unreal Pokémon" created by Retro & Chill.

#include "Components/Summary/SummaryPages.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Summary/SummaryScreenPage.h"
#include "Input/CommonUIInputTypes.h"

USummaryPages::USummaryPages(const FObjectInitializer &Initializer) : UCommonActivatableWidget(Initializer) {
    bIsBackHandler = true;
    bIsBackActionDisplayedInActionBar = true;
}

void USummaryPages::NativeConstruct() {
    Super::NativeConstruct();

    auto CreateBindArgs = [this](UInputAction *Action, auto Function, bool bDisplayInActionBar = false) {
        FBindUIActionArgs BindArgs(Action, FSimpleDelegate::CreateUObject(this, Function));
        BindArgs.bDisplayInActionBar = bDisplayInActionBar;
        return BindArgs;
    };

    InspectActionHandler = RegisterUIActionBinding(CreateBindArgs(SelectionAction, &USummaryPages::Select));
    NextPokemonActionHandle = RegisterUIActionBinding(CreateBindArgs(NextPokemonAction, &USummaryPages::NextPokemon));
    PreviousPokemonActionHandle =
        RegisterUIActionBinding(CreateBindArgs(PreviousPokemonAction, &USummaryPages::PreviousPokemon));
}

void USummaryPages::SetInitialPokemon(TConstArrayView<TScriptInterface<IPokemon>> Pokemon, int32 PartyIndex) {
    CurrentPokemon = FPokemonIterator(Pokemon, PartyIndex);
    if (OnPokemonChange.IsBound()) {
        OnPokemonChange.Execute(*CurrentPokemon);
    }
}

FOnPokemonChange &USummaryPages::GetOnPokemonChange() {
    return OnPokemonChange;
}

const TScriptInterface<IPokemon> &USummaryPages::GetCurrentPokemon() const {
    return *CurrentPokemon;
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

void USummaryPages::NextPokemon() {
    ++CurrentPokemon;
    OnPokemonChange.ExecuteIfBound(*CurrentPokemon);
}

void USummaryPages::PreviousPokemon() {
    --CurrentPokemon;
    OnPokemonChange.ExecuteIfBound(*CurrentPokemon);
}

FOnSelected &USummaryPages::GetOnSelected() {
    return OnSelected;
}

FOnScreenBackOut &USummaryPages::GetOnScreenBackOut() {
    return OnScreenBackOut;
}

UWidgetSwitcher *USummaryPages::GetPageSwitcher() const {
    return PageSwitcher;
}

bool USummaryPages::NativeOnHandleBackAction() {
    OnScreenBackOut.Broadcast();
    return true;
}