// "Unreal Pokémon" created by Retro & Chill.

#include "Components/Party/PokemonPanel.h"
#include "Components/Party/PokemonSelectionPane.h"
#include "Components/SelectableWidget.h"
#include "Pokemon/Pokemon.h"

void UPokemonPanel::SetOwner(USelectableWidget *NewOwner) {
    Owner = NewOwner;
}

const TScriptInterface<IPokemon> &UPokemonPanel::GetPokemon() const {
    return Pokemon;
}

void UPokemonPanel::SetPokemon(const TScriptInterface<IPokemon> &NewPokemon, int32 Index) {
    Pokemon = NewPokemon;
    PokemonIndex = Index;
    Refresh();
}

void UPokemonPanel::SwapPokemon(UPokemonPanel &Other) {
    Swap(Pokemon, Other.Pokemon);
}

bool UPokemonPanel::IsActivated() const {
    return PokemonIndex == 0;
}

bool UPokemonPanel::IsPokemonFainted() const {
    return Pokemon != nullptr ? Pokemon->IsFainted() : false;
}

bool UPokemonPanel::IsPanelSelected() const {
    return IsHovered();
}

bool UPokemonPanel::IsSwapping() const {
    auto SelectionPane = Cast<UPokemonSelectionPane>(Owner);
    if (SelectionPane == nullptr)
        return false;

    return SelectionPane->IsSwitching();
}

bool UPokemonPanel::IsPreselected() const {
    auto SelectionPane = Cast<UPokemonSelectionPane>(Owner);
    if (SelectionPane == nullptr)
        return false;

    auto &SwitchingIndex = SelectionPane->GetSwitchingIndex();
    return SwitchingIndex.Get(INDEX_NONE) == PokemonIndex;
}

void UPokemonPanel::Refresh() {
    RefreshVisuals();
}