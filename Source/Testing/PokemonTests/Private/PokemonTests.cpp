#include "PokemonTests.h"

#define LOCTEXT_NAMESPACE "FPokemonTestsModule"

void FPokemonTestsModule::StartupModule() {
    // No special setup needed
}

void FPokemonTestsModule::ShutdownModule() {
    // No special teardown needed
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FPokemonTestsModule, PokemonTests)
