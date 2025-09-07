// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

struct FPokemonDataManagedActions
{
    using FStartDataManager = void(__stdcall *)();

    FStartDataManager StartDataManager = nullptr;
};

/**
 *
 */
class POKEMONDATA_API FPokemonDataManagedCallbacks
{
    FPokemonDataManagedCallbacks() = default;
    ~FPokemonDataManagedCallbacks() = default;

  public:
    static FPokemonDataManagedCallbacks &Get();

    void SetCallbacks(const FPokemonDataManagedActions &InCallbacks);

    void CreateDataManager() const;

  private:
    FPokemonDataManagedActions Callbacks;
};
