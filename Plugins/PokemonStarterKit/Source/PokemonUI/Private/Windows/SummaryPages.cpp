// "Unreal Pokémon" created by Retro & Chill.

#include "Windows/SummaryPages.h"
#include "Components/WidgetSwitcher.h"

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

UWidgetSwitcher *USummaryPages::GetPageSwitcher() const {
    return PageSwitcher;
}

void USummaryPages::OnSelectionChange_Implementation(int32 OldIndex, int32 NewIndex) {
    Super::OnSelectionChange_Implementation(OldIndex, NewIndex);
    PageSwitcher->SetActiveWidgetIndex(NewIndex);
}