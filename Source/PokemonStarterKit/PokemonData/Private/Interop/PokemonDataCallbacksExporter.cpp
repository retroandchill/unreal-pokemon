// "Unreal Pokémon" created by Retro & Chill.

#include "Interop/PokemonDataCallbacksExporter.h"
#include "Interop/PokemonDataManagedCallbacks.h"

void UPokemonDataCallbacksExporter::SetCallbacks(const FPokemonDataManagedActions &InCallbacks)
{
    FPokemonDataManagedCallbacks::Get().SetCallbacks(InCallbacks);
}