// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "WarpingUtilities.generated.h"

struct FWarpDestination;
/**
 * Utility library for functions related to warping around the map
 */
UCLASS()
class GRIDBASED2D_API UWarpingUtilities : public UBlueprintFunctionLibrary {
	GENERATED_BODY()

public:
	/**
	 * Warp to the given location on the map
	 * @param WorldContext The world context object used to find the GameInstance
	 * @param Destination The destination information for the warp
	 */
	UFUNCTION(BlueprintCallable, Category = "Maps|Warping", meta = (WorldContext = WorldContext))
	static void WarpToMap(const UObject* WorldContext, const FWarpDestination& Destination);
};
