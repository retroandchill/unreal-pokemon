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
class GRIDBASED2D_API UCameraSubsystem : public UTickableWorldSubsystem {
    GENERATED_BODY()

  public:
    /**
     * The adjustment that needs to be made to the Z-Rotation to get the correct rotation in-game.
     */
    static constexpr double ZRotationAdjustment = 90.0;

    void OnWorldBeginPlay(UWorld &InWorld) override;
    void Tick(float DeltaTime) override;
    TStatId GetStatId() const override;

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

  private:
    /**
     * The dispatcher called when the camera angle is changed.
     */
    UPROPERTY(BlueprintAssignable, Category = Events)
    FOnCameraAngleChanged OnCameraAngleChanged;

    /**
     * The currently cached camera angle.
     */
    UPROPERTY(BlueprintGetter = GetCurrentCameraAngle, Category = Camera)
    FVector2D CurrentCameraAngle;

    UPROPERTY()
    TObjectPtr<APlayerCameraManager> PlayerCameraManager;
};
