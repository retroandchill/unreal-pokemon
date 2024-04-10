// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Characters/MoveCheckResult.h"
#include "UObject/Interface.h"
#include "GridMovable.generated.h"

class UGridBasedMovementComponent;
// This class does not need to be modified.
UINTERFACE(BlueprintType)
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

	/**
	 * Perform some additional checks on the specified target square. Can be used to override a previous check that
	 * didn't turn out as desired.
	 * movement check behavior doesn't find anything that would block movement already.
	 * @param TargetSquare The translated world-space position of the square in question to perform the checks on
	 * @param bBlockingHit Was this hit blocking or not
	 * @param HitComponent The actor that was hit in the event of a blocking hit
	 * @return Should we block movement based upon what we find?
	 */
	UFUNCTION(BlueprintNativeEvent, Category = Movement)
	bool PerformAdditionalMovementChecks(const FVector &TargetSquare, bool bBlockingHit, UPrimitiveComponent* HitComponent);
	
};
