// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Characters/FacingDirection.h"
#include "WarpDestination.generated.h"

/**
 * Struct used to store warp destination information
 */
USTRUCT(BlueprintType)
struct GRIDBASED2D_API FWarpDestination {
 GENERATED_BODY()

 /**
  * The map to warp to
  */
 UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Warping")
 TSoftObjectPtr<UWorld> Map;

 /**
  * The X-coordinate of the map to warp to
  */
 UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Warping")
 int32 X;

 /**
  * The Y-coordinate of the map to warp to
  */
 UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Warping")
 int32 Y;

 /**
  * The direction to face when warping. (Retain the current direction if not overriden.)
  */
 UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Warping", meta = (EditCondition = bOverride_Direction))
 EFacingDirection Direction;

 /**
  * Should the warp direction be explicitly set? If not retain the current direction.
  */
 UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Warping", meta = (InlineEditConditionToggle))
 bool bOverride_Direction;
};
