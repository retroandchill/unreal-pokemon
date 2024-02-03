#include "PokemonData.h"

constexpr auto GLoctextNamespace = "FPokemonDataModule";

void FPokemonDataModule::StartupModule() {
	// No custom setup needed
}

void FPokemonDataModule::ShutdownModule() {
	// No custom shutdown needed
}

IMPLEMENT_MODULE(FPokemonDataModule, PokemonData)
