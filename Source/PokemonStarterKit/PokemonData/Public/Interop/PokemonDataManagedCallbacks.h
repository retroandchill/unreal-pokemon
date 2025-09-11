// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "expected.hpp"
#include "StructUtils/StructView.h"

struct FPokemonDataManagedActions
{
    using FStartDataManager = void(__stdcall *)();
    using FGetEvolutionConditionClass = bool(__stdcall *)(const uint8 *, UScriptStruct **, FString *Error);

    FStartDataManager StartDataManager = nullptr;
    FGetEvolutionConditionClass GetEvolutionConditionClass = nullptr;
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

    tl::expected<UScriptStruct *, FString> GetEvolutionConditionClass(const uint8 *Handle) const;

  private:
    FPokemonDataManagedActions Callbacks;
};
