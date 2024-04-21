// "Unreal Pokémon" created by Retro & Chill.
#include "PokemonAssetsEditor.h"

#define LOCTEXT_NAMESPACE "FPokemonAssetsEditorModule"

void FPokemonAssetsEditorModule::StartupModule() {
    // No startup required
}

void FPokemonAssetsEditorModule::ShutdownModule() {
    // No shutdown required
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FPokemonAssetsEditorModule, PokemonAssetsEditor)