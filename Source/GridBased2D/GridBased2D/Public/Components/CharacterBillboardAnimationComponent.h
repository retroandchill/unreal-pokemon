// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CharacterBillboardAnimationComponent.generated.h"
#include "Components/ActorComponent.h"
#include "Components/GridBasedAnimationComponent.h"
#include "CoreMinimal.h"

class UMaterialBillboardComponent;
class IGridBasedMovement;
/**
 * Animation component that uses a Material Billboard Component
 */
UCLASS(ClassGroup = (Characters), meta = (BlueprintSpawnableComponent))
class GRIDBASED2D_API UCharacterBillboardAnimationComponent : public UActorComponent,
                                                              public IGridBasedAnimationComponent {
    GENERATED_BODY()

  public:
    void UpdateDirection(EFacingDirection Direction) override;
    bool IsMoveAnimationPlaying() const override;
    void StartMoveAnimation() override;
    bool CanStopMoving() const override;
    void StopMoveAnimation() override;

  protected:
    void BeginPlay() override;

  public:
    void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

    /**
     * Set up the material instance and attach it to the component
     */
    UFUNCTION(BlueprintCallable, Category = Rendering)
    void SetUpMaterialInstance();

    /**
     * Get the component used to render the billboard
     * @return The component used to render the billboard
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = Components)
    UMaterialBillboardComponent *GetBillboardComponent() const;

    /**
     * Get the component used to render the billboard
     * @return The component used to render the billboard
     */
    UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, Category = Components)
    void SetBillboardComponent(UMaterialBillboardComponent *NewBillboardComponent);

    /**
     * Get the individual material instance owned by this component
     * @return The individual material instance owned by this component
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = Components)
    UMaterialInstanceDynamic *GetMaterialInstance() const;

    /**
     * Set the individual material instance owned by this component
     * @param NewMaterialInstance The individual material instance owned by this component
     */
    UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, Category = Components)
    void SetMaterialInstance(UMaterialInstanceDynamic *NewMaterialInstance);

  private:
    /**
     * The base material used for rendering the billboard
     */
    UPROPERTY(EditAnywhere, Category = Rendering)
    TObjectPtr<UMaterialInterface> BaseMaterial;

    /**
     * The source texture for the component in question
     */
    UPROPERTY(EditAnywhere, Category = Rendering)
    TObjectPtr<UTexture2D> SourceTexture;

    /**
     * The component used to render the billboard
     */
    UPROPERTY(BlueprintGetter = GetBillboardComponent, BlueprintSetter = SetBillboardComponent, Category = Components)
    TObjectPtr<UMaterialBillboardComponent> BillboardComponent;

    /**
     * The individual material instance owned by this component
     */
    UPROPERTY(BlueprintGetter = GetMaterialInstance, BlueprintSetter = SetMaterialInstance, Category = Components)
    TObjectPtr<UMaterialInstanceDynamic> MaterialInstance;

    /**
     * The original framerate of the material while it's playing
     */
    UPROPERTY()
    float OriginalFrameRate = 0.f;

    /**
     * Is the flipbook currently playing
     */
    UPROPERTY(EditAnywhere, Category = Rendering)
    bool bIsPlaying = false;

    /**
     * The amount of time the animation has been playing for uninterrupted
     */
    TOptional<double> PlayingTime;

    /**
     * The total number of animation frames
     */
    UPROPERTY(EditAnywhere, AdvancedDisplay, Category = Rendering, meta = (UIMin = 1, ClampMin = 1))
    int32 TotalFrames = 4;

    /**
     * The frames that the animation can stop on
     */
    UPROPERTY(EditAnywhere, AdvancedDisplay, Category = Rendering)
    TSet<int32> ValidStopFrames = {0, 2};

    /**
     * The previously cached direction for the player (if there is one)
     */
    TOptional<EFacingDirection> CurrentDirection;

    /**
     * The name of the SourceTexture property in the material.
     */
    UPROPERTY(EditAnywhere, AdvancedDisplay, Category = "Rendering|Material Settings")
    FName SourceTexturePropertyName = TEXT("SourceTexture");

    /**
     * The name of the StartTime property in the material.
     */
    UPROPERTY(EditAnywhere, AdvancedDisplay, Category = "Rendering|Material Settings")
    FName StartTimePropertyName = TEXT("StartTime");

    /**
     * The name of the FrameRate property in the material.
     */
    UPROPERTY(EditAnywhere, AdvancedDisplay, Category = "Rendering|Material Settings")
    FName FrameRatePropertyName = TEXT("FrameRate");

    /**
     * The name of the Direction property in the material.
     */
    UPROPERTY(EditAnywhere, AdvancedDisplay, Category = "Rendering|Material Settings")
    FName DirectionPropertyName = TEXT("Direction");
};
