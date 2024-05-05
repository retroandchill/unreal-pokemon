// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "AbilityIndex.generated.h"

/**
 * Thin wrapper around the numeric ability index of the Pokémon.
 */
USTRUCT(BlueprintType)
struct POKEMONDATA_API FAbilityIndex {
    GENERATED_BODY()

    /**
     * The internal index used for this class.
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category = Abilities)
    int32 Index = 0;

    FAbilityIndex();
    
    FAbilityIndex(int32 IndexIn);
};
