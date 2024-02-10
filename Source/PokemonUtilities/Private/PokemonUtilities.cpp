#include "PokemonUtilities.h"

#define LOCTEXT_NAMESPACE "FPokemonUtilitiesModule"

void FPokemonUtilitiesModule::StartupModule()
{
    // No startup needed
}

void FPokemonUtilitiesModule::ShutdownModule()
{
    // No shutdown needed
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FPokemonUtilitiesModule, PokemonUtilities)