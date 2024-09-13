// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"

#include "CameraSubsystem.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnCameraAngleChanged, const FVector2D&);

/**
 * Subsystem for handling the camera angle among other aspects of the system.
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
     * Bind a delegate to the camera angle changed event.
     * @param Binding The delegate to bind to the event
     * @return The handle for the bound event.
     */
    FDelegateHandle BindToGetOnCameraAngleChanged(FOnCameraAngleChanged::FDelegate&& Binding);

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
    FOnCameraAngleChanged OnCameraAngleChanged;

    /**
     * The currently cached camera angle.
     */
    UPROPERTY(BlueprintGetter = GetCurrentCameraAngle, Category = Camera)
    FVector2D CurrentCameraAngle;

    UPROPERTY()
    TObjectPtr<APlayerCameraManager> PlayerCameraManager;
};
