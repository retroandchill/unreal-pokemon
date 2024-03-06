// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "DirectionalSprite.generated.h"

class UPaperFlipbook;

/**
 * Struct used to house the movement information for a character
 */
USTRUCT(BlueprintType)
struct GRIDBASED2D_API FDirectionalSprite {
	GENERATED_BODY()

	/**
	 * The flipbook sprite used to house this character set
	 */
	UPROPERTY(EditAnywhere, Category = "Character|Sprites")
	TObjectPtr<UPaperFlipbook> Flipbook;

	/**
	 * The set of valid frames this sprite can stop on
	 */
	UPROPERTY(EditAnywhere, Category = "Character|Sprites")
	TSet<int32> ValidStopFrames = { 0, 2 };
};
