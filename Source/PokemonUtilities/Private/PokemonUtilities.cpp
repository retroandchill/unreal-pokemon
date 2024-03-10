#include "PokemonUtilities.h"

constexpr auto* GLoctextNamespace = "FPokemonUtilitiesModule";

void FPokemonUtilitiesModule::StartupModule() {
	// No startup needed
}

void FPokemonUtilitiesModule::ShutdownModule() {
	// No shutdown needed
}

IMPLEMENT_MODULE(FPokemonUtilitiesModule, PokemonUtilities)
