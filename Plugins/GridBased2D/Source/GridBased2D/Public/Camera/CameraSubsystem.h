// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "CameraSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCameraAngleChanged, FVector2D, Angle);

/**
 * Subystem for handling the camera angle among other aspects of the system.
 */
UCLASS()
class GRIDBASED2D_API UCameraSubsystem : public UWorldSubsystem {
    GENERATED_BODY()

public:
    /**
     * Get the dispatcher called when the camera angle is changed.
     * @return The dispatcher called when the camera angle changes.
     */
    FOnCameraAngleChanged &GetOnCameraAngleChanged();

    /**
     * Get the currently cached camera angle.
     * @return The currently cached camera angle.
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = Camera)
    FVector2D GetCurrentCameraAngle() const;

    /**
     * Notify the game that the camera angle has changed
     * @param NewAngle The new angle that the camera was rotated to.
     */
    UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, Category = Events)
    void SetCurrentCameraAngle(FVector2D NewAngle);

private:
    /**
     * The dispatcher called when the camera angle is changed.
     */
    UPROPERTY(BlueprintAssignable, Category = Events)
    FOnCameraAngleChanged OnCameraAngleChanged;

    /**
     * The currently cached camera angle.
     */
    UPROPERTY(BlueprintGetter=GetCurrentCameraAngle, BlueprintSetter=SetCurrentCameraAngle, Category = Camera)
    FVector2D CurrentCameraAngle;
    
};
