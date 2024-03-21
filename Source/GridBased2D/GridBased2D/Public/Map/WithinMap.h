// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "WithinMap.generated.h"

// This class does not need to be modified.
UINTERFACE(NotBlueprintable)
class UWithinMap : public UInterface {
	GENERATED_BODY()
};

/**
 * Interface for classes that are within a given map.
 */
class GRIDBASED2D_API IWithinMap {
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/**
	 * Get the current grid position of this object
	 * @return The current grid position of the object
	 */
	virtual FIntVector2 GetCurrentPosition() const = 0;

	/**
	 * Warp the character to the given position
	 * @param X The X-coordinate to place the character at
	 * @param Y The Y-coordinate to place the character at
	 */
	UFUNCTION(BlueprintCallable, Category = "Character|Movement")
	virtual void WarpToLocation(int32 X, int32 Y) = 0;
};
