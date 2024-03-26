// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GridMovable.generated.h"

class UGridBasedMovementComponent;
// This class does not need to be modified.
UINTERFACE()
class UGridMovable : public UInterface {
	GENERATED_BODY()
};

/**
 * Interface for an actor that is movable within the grid.
 */
class GRIDBASED2D_API IGridMovable {
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Movement)
	UGridBasedMovementComponent* GetGridBasedMovementComponent() const;
	
};
