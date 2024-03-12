// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "LevelUpMove.generated.h"

/**
 * Represents a move learned upon level up
 */
USTRUCT(BlueprintType)
struct POKEMONDATA_API FLevelUpMove {
	GENERATED_BODY()

	/**
	 * The level this move is learned at (0 = learned upon evolution)
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Moves", meta = (UIMin = 0, ClampMin = 0))
	int Level = 1;

	/**
	 * The ID of the move that is learned at that level
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Moves")
	FName Move;
};
