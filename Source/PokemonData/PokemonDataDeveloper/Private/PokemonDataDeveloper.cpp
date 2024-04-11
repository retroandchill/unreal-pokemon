// "Unreal Pokémon" created by Retro & Chill.
#include "PokemonDataDeveloper.h"

#include "Pins/PokemonDataGraphPinFactory.h"

#define LOCTEXT_NAMESPACE "FPokemonDataDeveloperModule"

void FPokemonDataDeveloperModule::StartupModule() {
    auto PinFactory = MakeShared<FPokemonDataGraphPinFactory>();
    FEdGraphUtilities::RegisterVisualPinFactory(MoveTemp(PinFactory));
}

void FPokemonDataDeveloperModule::ShutdownModule() {
    // No special teardown needed
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FPokemonDataDeveloperModule, PokemonDataDeveloper)
