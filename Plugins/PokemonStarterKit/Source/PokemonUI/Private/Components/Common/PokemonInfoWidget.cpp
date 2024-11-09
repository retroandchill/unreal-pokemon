// "Unreal Pokémon" created by Retro & Chill.

#include "Components/Common/PokemonInfoWidget.h"

void UPokemonInfoWidget::Refresh_Implementation(const TScriptInterface<IPokemon> &Pokemon) {
    // No native implementation in this class
}

void UPokemonInfoWidget::SetPokemon(const TScriptInterface<IPokemon> &NewPokemon) {
    Pokemon = NewPokemon;
    OnPokemonSet(NewPokemon);
}

void UPokemonInfoWidget::OnPokemonSet_Implementation(const TScriptInterface<IPokemon> &NewPokemon) {
    // No native implementation in the base class
}