// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "Characters/FacingDirection.h"
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GridBasedMovement.generated.h"

// This class does not need to be modified.
UINTERFACE(BlueprintType, NotBlueprintable)
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
};
