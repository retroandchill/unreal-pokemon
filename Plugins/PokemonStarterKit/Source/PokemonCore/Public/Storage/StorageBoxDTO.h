// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Pokemon/PokemonDTO.h"

/**
 * The underlying storage type used to save and load a storage box
 */
USTRUCT(BlueprintType)
struct POKEMONCORE_API FStorageBoxDTO {
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, Category = Information)
    FText DisplayName;

    UPROPERTY(EditAnywhere, Category = Contents)
    TArray<TOptional<FPokemonDTO>> StoredPokemon;
};
