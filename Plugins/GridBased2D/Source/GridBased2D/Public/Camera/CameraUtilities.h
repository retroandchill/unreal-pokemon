// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CameraUtilities.generated.h"

/**
 * Utility library for handling camera related operations.
 */
UCLASS()
class GRIDBASED2D_API UCameraUtilities : public UBlueprintFunctionLibrary {
    GENERATED_BODY()

public:
    /**
     * Get the desired rotation of the camera for the given position.
     * @param Rotation The rotation for the camera arm
     * @return The desired component rotation
     */
    UFUNCTION(BlueprintPure, Category="Camera")
    static FRotator TranslateCameraRotation(const FVector2D& Rotation);

};
