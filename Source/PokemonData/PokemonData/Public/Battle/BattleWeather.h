// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "BattleWeather.generated.h"

/**
 * Represents the weather effects that occur in battle
 */
USTRUCT(BlueprintType, meta = (DatabaseType="Hardcoded"))
struct POKEMONDATA_API FBattleWeather : public FTableRowBase {
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

	/**
	 * The animation displayed when the weather is active. When it displays depends on the specific implementation.
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Visuals", meta = (MustImplement = "AnimationBase"))
	TSubclassOf<AActor> Animation;
};
