// Created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "GrowthRate.h"
#include "UObject/Object.h"
#include "GrowthRateData.generated.h"

/**
 * Represents the data row used to retrieve the exp growth curves
 */
USTRUCT(BlueprintType)
struct POKEMONDATA_API FGrowthRateData : public FTableRowBase {
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
	 * The C++ or Blueprint Class that implements Growth Rate that corresponds to this Growth rate
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (MustImplement = "GrowthRate"))
	TSubclassOf<UObject> ImplementationClass;
};
