// "Unreal Pokémon" created by Retro & Chill.
#include "PokemonUI.h"

DEFINE_LOG_CATEGORY(LogPokemonUI)

constexpr auto GLoctextNamespace = "FPokemonUIModule";

void FPokemonUIModule::StartupModule() {
    // No special startup needed
}

void FPokemonUIModule::ShutdownModule() {
    // No special shutdown needed
}

IMPLEMENT_MODULE(FPokemonUIModule, PokemonUI)
