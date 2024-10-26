#include "PokemonBlueprintNodes.h"
#include "Data/PokemonDataGraphPinFactory.h"

void FPokemonBlueprintNodesModule::StartupModule() {
    auto PinFactory = MakeShared<FPokemonDataGraphPinFactory>();
    FEdGraphUtilities::RegisterVisualPinFactory(PinFactory);
}

void FPokemonBlueprintNodesModule::ShutdownModule() {
    // No shutdown procedure
}

IMPLEMENT_MODULE(FPokemonBlueprintNodesModule, PokemonBlueprintNodes)