// Created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Habitat.generated.h"

/**
 * Represents the Body Shape of a Pokémon. Used when searching in the Pokédex.
 */
USTRUCT(BlueprintType)
struct POKEMONDATA_API FHabitat : public FTableRowBase {
	GENERATED_BODY()

	/**
   * The internal ID used for lookup by the game
   */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FName ID;

	/**
	 * The name that is displayed to the player
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FText RealName;
};
