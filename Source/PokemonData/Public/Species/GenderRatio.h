// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "GenderRatio.generated.h"

/**
 * Represents the Gender Ratio for a Specific Pokémon
 */
USTRUCT(BlueprintType, meta = (DatabaseType="Hardcoded"))
struct POKEMONDATA_API FGenderRatio : public FTableRowBase {
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
	 * If true this species is genderless.
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Gender Determination")
	bool IsGenderless;

	/**
	 * The chance this species has of being female out of 255. (Ignored if "Is Genderless" is set to true)
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Gender Determination", meta = (UIMin = 0, ClampMin = 0, UIMax = 255, ClampMax = 255))
	int32 FemaleChance;
};
