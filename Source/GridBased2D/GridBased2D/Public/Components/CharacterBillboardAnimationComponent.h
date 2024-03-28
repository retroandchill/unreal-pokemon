// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/GridBasedAnimationComponent.h"
#include "CharacterBillboardAnimationComponent.generated.h"

class UMaterialBillboardComponent;
class IGridBasedMovement;
/**
 * Animation component that uses a Material Billboard Component
 */
UCLASS(ClassGroup=(Characters), meta=(BlueprintSpawnableComponent))
class GRIDBASED2D_API UCharacterBillboardAnimationComponent : public UActorComponent, public IGridBasedAnimationComponent {
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
	UMaterialBillboardComponent* GetBillboardComponent() const;

	/**
	 * Get the component used to render the billboard
	 * @return The component used to render the billboard
	 */
	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, Category = Components)
	void SetBillboardComponent(UMaterialBillboardComponent* NewBillboardComponent);

	/**
	 * Get the individual material instance owned by this component
	 * @return The individual material instance owned by this component
	 */
	UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = Components)
	UMaterialInstanceDynamic* GetMaterialInstance() const;

	/**
	 * Set the individual material instance owned by this component
	 * @param NewMaterialInstance The individual material instance owned by this component
	 */
	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, Category = Components)
	void SetMaterialInstance(UMaterialInstanceDynamic* NewMaterialInstance);

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
	 * The name of the SourceTexture property in the material.
	 */
	UPROPERTY(EditAnywhere, Category = "Rendering|Material Settings")
	FName SourceTexturePropertyName = TEXT("SourceTexture");

	/**
	 * The name of the FrameRate property in the material.
	 */
	UPROPERTY(EditAnywhere, Category = "Rendering|Material Settings")
	FName FrameRatePropertyName = TEXT("FrameRate");

	/**
	 * The name of the Direction property in the material.
	 */
	UPROPERTY(EditAnywhere, Category = "Rendering|Material Settings")
	FName DirectionPropertyName = TEXT("Direction");
};
