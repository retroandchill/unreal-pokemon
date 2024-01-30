// Created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "TargetCount.h"
#include "UObject/Object.h"
#include "Target.generated.h"

/**
 * Handles target processing for a move
 */
USTRUCT(BlueprintType)
struct POKEMONDATA_API FTarget : public FTableRowBase {
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
	 * How many targets does this move have? 
	 */
	UPROPERTY(BlueprintType, EditAnywhere)
	ETargetCount NumTargets;

	/**
	 * Does this move target a Pokémon on the opposing side?
	 */
	UPROPERTY(BlueprintType, EditAnywhere)
	bool TargetsFoe = false;

	/**
	 * Does this move target both sides of the field?
	 */
	UPROPERTY(BlueprintType, EditAnywhere)
	bool TargetsAll = false;

	/**
	 * Does this move affect the opposing side of the field?
	 */
	UPROPERTY(BlueprintType, EditAnywhere)
	bool AffectsFoeSide = false;

	/**
	 * Is this move able to hit an opponent that is not directly adjacent to the target?
	 */
	UPROPERTY(BlueprintType, EditAnywhere)
	bool LongRange = false;
};
