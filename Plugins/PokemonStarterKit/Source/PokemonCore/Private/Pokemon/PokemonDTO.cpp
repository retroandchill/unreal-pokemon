// "Unreal Pokémon" created by Retro & Chill.

#include "Pokemon/PokemonDTO.h"
#include "Lookup/InjectionUtilities.h"

TScriptInterface<IPokemon> FPokemonDTO::CreatePokemon(UObject *Outer) const {
    return UnrealInjector::NewInjectedDependency<IPokemon>(Outer, *this);
}