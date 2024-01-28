// Created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "NatureStatChange.h"
#include "UObject/Object.h"
#include "Nature.generated.h"

/**
 * Represents one of the possible Pokémon natures
 */
USTRUCT(BlueprintType)
struct POKEMONDATA_API FNature : public FTableRowBase {
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

	/**
	 * The list of stat changes to be applied
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TArray<FNatureStatChange> StatChanges;
};
