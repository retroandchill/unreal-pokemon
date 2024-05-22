// "Unreal Pokémon" created by Retro & Chill.


#include "Components/Summary/PokemonMovesPage.h"

void UPokemonMovesPage::RefreshInfo_Implementation(const TScriptInterface<IPokemon> &Pokemon) {
    Super::RefreshInfo_Implementation(Pokemon);
    DrawPokemonMoves.Broadcast(Pokemon);
}

bool UPokemonMovesPage::CanSelectOnPage_Implementation() const {
    return true;
}