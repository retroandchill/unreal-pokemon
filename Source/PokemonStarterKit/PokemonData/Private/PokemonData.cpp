// "Unreal Pokémon" created by Retro & Chill.
#include "PokemonData.h"
#include "CSManager.h"
#include "Interop/PokemonDataManagedCallbacks.h"

void FPokemonDataModule::StartupModule()
{
    FCoreDelegates::OnPostEngineInit.AddLambda([] {
        FPokemonDataManagedCallbacks::Get().CreateDataManager();

#if WITH_EDITOR
        static FName PokemonData = TEXT("Pokemon.Data");
        auto &CSManager = UCSManager::Get();
        CSManager.OnManagedAssemblyLoadedEvent().AddLambda([](const FName &AssemblyPath) {
            if (AssemblyPath != PokemonData)
            {
                return;
            }

            FPokemonDataManagedCallbacks::Get().CreateDataManager();
        });
#endif
    });
}

void FPokemonDataModule::ShutdownModule()
{
    // No custom shutdown needed
}

IMPLEMENT_MODULE(FPokemonDataModule, PokemonData)
