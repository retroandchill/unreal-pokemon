// Created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "PaperSprite.h"
#include "UObject/Object.h"
#include "Status.generated.h"

/**
 * Represents the data for a status condition
 */
USTRUCT(BlueprintType)
struct POKEMONDATA_API FStatus : public FTableRowBase {
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
	 * The animation displayed when this status is inflicted and when its effect goes off
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (MustImplement = "AnimationBase"))
	TSubclassOf<AActor> Animation;

	/**
	 * The icon displayed in the UI for this status condition
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	UPaperSprite *Icon;
};
