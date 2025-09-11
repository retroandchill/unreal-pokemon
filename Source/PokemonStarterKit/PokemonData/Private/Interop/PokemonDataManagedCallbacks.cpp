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

tl::expected<UScriptStruct *, FString> FPokemonDataManagedCallbacks::GetEvolutionConditionClass(
    const uint8 *Handle) const
{
    FString Error;
    if (UScriptStruct * Result; Callbacks.GetEvolutionConditionClass(Handle, &Result, &Error))
    {
        return Result;
    }

    return tl::unexpected(MoveTemp(Error));
}