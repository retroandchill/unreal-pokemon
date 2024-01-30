// Created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "BattleWeather.generated.h"

/**
 * Represents the weather effects that occur in battle
 */
USTRUCT(BlueprintType)
struct POKEMONDATA_API FBattleWeather : public FTableRowBase {
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
