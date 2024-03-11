// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "PaperSprite.h"
#include "UObject/Object.h"
#include "Status.generated.h"

/**
 * Represents the data for a status condition
 */
USTRUCT(BlueprintType, meta = (DatabaseType="Hardcoded"))
struct POKEMONDATA_API FStatus : public FTableRowBase {
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
	 * The animation displayed when this status is inflicted and when its effect goes off
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Visuals", meta = (MustImplement = "AnimationBase"))
	TSubclassOf<AActor> Animation;
};
