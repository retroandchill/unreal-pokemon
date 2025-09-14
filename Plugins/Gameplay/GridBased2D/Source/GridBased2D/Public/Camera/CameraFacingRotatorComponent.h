// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "CameraFacingRotatorComponent.generated.h"

/**
 * Delegate that is dispatched when a component is rotated
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnComponentRotated, const FVector2D &, CameraAngle);

/**
 * Camera component used to rotate the player towards the camera
 */
UCLASS(ClassGroup = (Camera), meta = (BlueprintSpawnableComponent))
class GRIDBASED2D_API UCameraFacingRotatorComponent : public UActorComponent
{
    GENERATED_BODY()

  public:
    /**
     * Sets default values for this component's properties.
     */
    UCameraFacingRotatorComponent();

  protected:
    void BeginPlay() override;

    /**
     * Get the actively rotated component.
     * @return The component used for rotation
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly)
    USceneComponent *GetRotatedComponent() const
    {
        return RotatedComponent;
    }

    /**
     * Set the rotated component.
     * @param NewRotatedComponent The component used for rotation
     */
    UFUNCTION(BlueprintCallable, BlueprintINTERNALUseOnly)
    void SetRotatedComponent(USceneComponent *NewRotatedComponent)
    {
        RotatedComponent = NewRotatedComponent;
    }

  private:
    void AdjustCameraRotation(const FVector2D &Angle) const;

    /**
     * The component used for rotation
     */
    UPROPERTY(BlueprintGetter = GetRotatedComponent, BlueprintSetter = SetRotatedComponent, Category = Components)
    TObjectPtr<USceneComponent> RotatedComponent;

    /**
     * Called when the component's rotation is changed
     */
    UPROPERTY(BlueprintAssignable, Category = Events)
    FOnComponentRotated OnComponentRotated;
};