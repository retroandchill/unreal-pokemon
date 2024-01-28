// Created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "EggGroupType.h"
#include "EggGroup.generated.h"

/**
 * Represents an Egg Group for the game
 */
USTRUCT(BlueprintType)
struct POKEMONDATA_API FEggGroup : public FTableRowBase  {
 GENERATED_BODY()
 
 /**
   * The internal ID of this Egg Group
   */
 UPROPERTY(BlueprintReadOnly, EditAnywhere)
 FName ID;

 /**
  * The name of this group if this were to be displayed to the player
  */
 UPROPERTY(BlueprintReadOnly, EditAnywhere)
 FText RealName;

 /**
  * What kind of Egg Group is this
  */
 UPROPERTY(BlueprintReadOnly, EditAnywhere)
 EEggGroupType Type;
 
};
