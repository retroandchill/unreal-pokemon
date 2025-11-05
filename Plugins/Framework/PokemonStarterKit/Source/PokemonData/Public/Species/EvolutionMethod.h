// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

#include "EvolutionMethod.generated.h"

/**
 * Represents an evolution method on an individual species
 */
USTRUCT(BlueprintType)
struct POKEMONDATA_API FEvolutionMethod {
    GENERATED_BODY()

    /**
     * The species the Pokémon will evolve into
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Evolution",
              meta = (GetOptions = "PokemonData.SpeciesHelper.GetSpeciesNames"))
    FName Species;

    /**
     * The method of evolution
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Evolution",
              meta = (GetOptions = "PokemonData.EvolutionHelper.GetEvolutionNames"))
    FName Method;

    /**
     * The condition that must be met (value depends on the method in question)
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Evolution")
    FString Parameter;
};
