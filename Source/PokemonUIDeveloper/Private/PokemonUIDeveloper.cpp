// "Unreal Pokémon" created by Retro & Chill.
#include "PokemonUIDeveloper.h"

#define LOCTEXT_NAMESPACE "FPokemonUIDeveloperModule"

void FPokemonUIDeveloperModule::StartupModule() {
	// No special setup needed
}

void FPokemonUIDeveloperModule::ShutdownModule() {
	// No special teardown needed
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FPokemonUIDeveloperModule, PokemonUIDeveloper)
