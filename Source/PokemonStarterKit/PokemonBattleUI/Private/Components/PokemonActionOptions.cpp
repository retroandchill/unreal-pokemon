// "Unreal Pokémon" created by Retro & Chill.

#include "Components/PokemonActionOptions.h"

const TScriptInterface<IBattler> &UPokemonActionOptions::GetCurrentBattler() const {
    return CurrentBattler;
}

void UPokemonActionOptions::SetBattler(const TScriptInterface<IBattler> &Battler) {
    CurrentBattler = Battler;
}