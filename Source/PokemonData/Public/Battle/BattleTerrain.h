// Created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "BattleTerrain.generated.h"

/**
 * In-battle terrain effects caused by moves like Electric Terrain.
 */
USTRUCT(BlueprintType, meta = (DatabaseType="Hardcoded"))
struct POKEMONDATA_API FBattleTerrain : public FTableRowBase {
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
	 * The animation displayed when the weather is active. When it displays depends on the specific implementation.
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (MustImplement = "AnimationBase"))
	TSubclassOf<AActor> Animation;
};
