// Created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Weather.generated.h"

/**
 * Represents a weather effect that may be present on a map
 */
USTRUCT(BlueprintType, meta = (DatabaseType="Hardcoded"))
struct POKEMONDATA_API FWeather : public FTableRowBase {
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
	 * The category used to determine various things in the world:
	 * - Determines the in-battle weather.
	 * - Some abilities reduce the encounter rate in certain categories of weather.
	 * - Some evolution methods check the current weather's category.
	 * - "Rain" auto-waters berry plants.
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FName Category;

	/**
	 * The Actor class that handles actually displaying the weather on the map
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (MustImplement = "WeatherVisuals"))
	TSubclassOf<AActor> ImplementationClass;
};
