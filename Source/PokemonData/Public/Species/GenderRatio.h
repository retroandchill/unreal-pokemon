// Created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "GenderRatio.generated.h"

/**
 * Represents the Gender Ratio for a Specific Pokémon
 */
USTRUCT(BlueprintType)
struct POKEMONDATA_API FGenderRatio : public FTableRowBase {
  GENERATED_BODY()

  /**
   * The internal ID of this Gender Ratio
   */
  UPROPERTY(BlueprintReadOnly, EditAnywhere)
	 FName ID;

  /**
   * The name of this ratio if this were to be displayed to the player
   */
  UPROPERTY(BlueprintReadOnly, EditAnywhere)
  FText RealName;

  /**
   * If true this species is genderless.
   */
  UPROPERTY(BlueprintReadOnly, EditAnywhere)
  bool IsGenderless;

  /**
   * The chance this species has of being female out of 255. (Ignored if "Is Genderless" is set to true)
   */
  UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (UIMin = 0, ClampMin = 0, UIMax = 255, ClampMax = 255))
  int32 FemaleChance;
};
