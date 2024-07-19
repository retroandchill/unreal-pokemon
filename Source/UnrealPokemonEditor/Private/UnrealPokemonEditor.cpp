#include "UnrealPokemonEditor.h"

DEFINE_LOG_CATEGORY(LogUnrealPokemonEditor)

void FUnrealPokemonEditorModule::StartupModule() {
    // No startup procedure
}

void FUnrealPokemonEditorModule::ShutdownModule() {
    // No shutdown procedure
}
    
IMPLEMENT_MODULE(FUnrealPokemonEditorModule, UnrealPokemonEditor)