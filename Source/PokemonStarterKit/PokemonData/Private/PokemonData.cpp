// "Unreal Pokémon" created by Retro & Chill.
#include "PokemonData.h"
#include "Interop/PokemonDataManagedCallbacks.h"

void FPokemonDataModule::StartupModule()
{
    FCoreDelegates::OnPostEngineInit.AddLambda([] { FPokemonDataManagedCallbacks::Get().CreateDataManager(); });
}

void FPokemonDataModule::ShutdownModule()
{
    // No custom shutdown needed
}

IMPLEMENT_MODULE(FPokemonDataModule, PokemonData)
