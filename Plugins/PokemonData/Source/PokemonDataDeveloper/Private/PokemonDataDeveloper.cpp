// "Unreal Pokémon" created by Retro & Chill.
#include "PokemonDataDeveloper.h"

#define LOCTEXT_NAMESPACE "FPokemonDataDeveloperModule"

void FPokemonDataDeveloperModule::StartupModule() {
}

void FPokemonDataDeveloperModule::ShutdownModule() {
    // No special teardown needed
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FPokemonDataDeveloperModule, PokemonDataDeveloper)
