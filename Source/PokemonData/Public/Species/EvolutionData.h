// Created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "EvolutionData.generated.h"

/**
 * The data related with how a Pokémon evolves
 */
USTRUCT()
struct POKEMONDATA_API FEvolutionData : public FTableRowBase {
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
	 * The C++ or Blueprint Class that implements Evolution Base that corresponds to this evolution method
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (MustImplement = "EvolutionBase"))
	TSubclassOf<UObject> ImplementationClass;
};
