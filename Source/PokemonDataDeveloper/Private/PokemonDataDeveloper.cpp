// "Unreal Pokémon" created by Retro & Chill.
#include "PokemonDataDeveloper.h"

#define LOCTEXT_NAMESPACE "FPokemonDataDeveloperModule"

void FPokemonDataDeveloperModule::StartupModule() {
	// No special setup needed
}

void FPokemonDataDeveloperModule::ShutdownModule() {
	// No special teardown needed
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FPokemonDataDeveloperModule, PokemonDataDeveloper)
