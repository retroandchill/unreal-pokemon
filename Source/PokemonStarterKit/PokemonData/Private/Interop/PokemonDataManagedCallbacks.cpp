// "Unreal Pokémon" created by Retro & Chill.

#include "Interop/PokemonDataManagedCallbacks.h"

FPokemonDataManagedCallbacks &FPokemonDataManagedCallbacks::Get()
{
    static FPokemonDataManagedCallbacks Instance;
    return Instance;
}

void FPokemonDataManagedCallbacks::SetCallbacks(const FPokemonDataManagedActions &InCallbacks)
{
    Callbacks = InCallbacks;
}

void FPokemonDataManagedCallbacks::CreateDataManager() const
{
    Callbacks.StartDataManager();
}