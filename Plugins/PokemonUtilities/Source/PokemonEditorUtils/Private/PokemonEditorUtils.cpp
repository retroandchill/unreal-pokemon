// "Unreal Pokémon" created by Retro & Chill.
#include "PokemonEditorUtils.h"

constexpr auto GLoctextNamespace = "FPokemonEditorUtilsModule";

void FPokemonEditorUtilsModule::StartupModule() {
    // No startup needed
}

void FPokemonEditorUtilsModule::ShutdownModule() {
    // No special shutdown code
}

IMPLEMENT_MODULE(FPokemonEditorUtilsModule, PokemonEditorUtils)
