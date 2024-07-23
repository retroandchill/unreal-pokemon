// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Characters/FacingDirection.h"
#include "UObject/Interface.h"

#include "GridBasedMovement.generated.h"

// This class does not need to be modified.
UINTERFACE(NotBlueprintable, BlueprintType)
class UGridBasedMovement : public UInterface {
    GENERATED_BODY()
};

/**
 * Interface for the Grid-Based Movement Actor component
 */
class GRIDBASED2D_API IGridBasedMovement {
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
  public:
    /**
     * Get the direction this character is actively facing
     * @return The direction the character is facing.
     */
    UFUNCTION(BlueprintCallable, Category = "Character|Movement")
    virtual EFacingDirection GetDirection() const = 0;

    /**
     * Get if the character is actively moving or not
     * @return Is the character actively moving?
     */
    UFUNCTION(BlueprintCallable, Category = "Character|Movement")
    virtual bool IsMoving() const = 0;

    /**
     * Perform the movement action when receiving the input
     * @param MovementVector The input information received
     */
    UFUNCTION(BlueprintCallable, Category = "Character|Movement", meta = (AutoCreateRefTerm = MovementVector))
    virtual void MoveInput(const FVector2D &MovementVector) = 0;

    /**
     * Perform the face direction action when receiving the input
     * @param MovementVector The input information received
     */
    UFUNCTION(BlueprintCallable, Category = "Character|Movement", meta = (AutoCreateRefTerm = MovementVector))
    virtual void TurnInput(const FVector2D &MovementVector) = 0;
};
