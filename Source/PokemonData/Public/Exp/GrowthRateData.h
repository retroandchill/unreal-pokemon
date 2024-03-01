// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GrowthRateData.generated.h"

/**
 * Represents the data row used to retrieve the exp growth curves
 */
USTRUCT(BlueprintType, meta = (DatabaseType="Hardcoded"))
struct POKEMONDATA_API FGrowthRateData : public FTableRowBase {
	GENERATED_BODY()

	/**
     * The internal ID used for lookup by the game
     */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Basic")
	FName ID;

	/**
	 * The name that is displayed to the player
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Basic")
	FText RealName;
	
};
