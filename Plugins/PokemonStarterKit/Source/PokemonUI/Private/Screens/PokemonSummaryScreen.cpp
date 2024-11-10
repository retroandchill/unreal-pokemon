// "Unreal Pokémon" created by Retro & Chill.

#include "Screens/PokemonSummaryScreen.h"
#include "CommonButtonBase.h"
#include "Components/Common/PokemonSelectionPaneBase.h"
#include "Components/Summary/SummaryPages.h"
#include "Components/Summary/SummaryScreenPage.h"
#include "Components/Summary/SummaryTabWidget.h"
#include "Utilities/RPGMenuUtilities.h"

DEFINE_INJECTABLE_DEPENDENCY(UPokemonSummaryScreen)

UPokemonSummaryScreen *UPokemonSummaryScreen::AddPokemonSummaryScreenToStack(const UObject *WorldContextObject) {
    return URPGMenuUtilities::InjectScreenToStack<UPokemonSummaryScreen>(WorldContextObject).GetPtrOrNull();
}

void UPokemonSummaryScreen::NativePreConstruct() {
    Super::NativePreConstruct();

    if (SummaryTabs != nullptr && SummaryPages != nullptr) {
        SummaryTabs->SetSummaryPages(SummaryPages);
    }
}

void UPokemonSummaryScreen::NativeConstruct() {
    Super::NativeConstruct();
    SummaryPages->GetOnPokemonChange().BindUObject(this, &UPokemonSummaryScreen::SetPokemon);
}

void UPokemonSummaryScreen::SetInitialPokemon(const TScriptInterface<ITrainer> &Trainer, int32 InitialIndex) {
    SetInitialPokemon(Trainer->GetParty(), InitialIndex);
}

void UPokemonSummaryScreen::SetInitialPokemon(TConstArrayView<TScriptInterface<IPokemon>> Party, int32 InitialIndex) {
    PokemonSelection->SetPokemonToDisplay(Party);
    PokemonSelection->SetIndex(InitialIndex);
    PokemonSelection->GetSelectedOption()->SetIsSelected(true);
    PokemonSelection->ActivateWidget();
}

USummaryPages *UPokemonSummaryScreen::GetSummaryPages() const {
    return SummaryPages;
}

UPokemonSelectionPaneBase *UPokemonSummaryScreen::GetPokemonSelection() const {
    return PokemonSelection;
}

void UPokemonSummaryScreen::SetPokemon(const TScriptInterface<IPokemon> &Pokemon) {
    SummaryPages->SetPokemon(Pokemon);
}