// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "GridBasedAnimationComponent.h"
#include "Components/ActorComponent.h"
#include "CharsetAnimationComponent.generated.h"

class UPaperFlipbook;
class UCharset;
class UPaperFlipbookComponent;
/**
 * Component used to manage charset animations
 */
UCLASS(ClassGroup=(Characters), meta=(BlueprintSpawnableComponent))
class GRIDBASED2D_API UCharsetAnimationComponent : public UActorComponent, public IGridBasedAnimationComponent {
	GENERATED_BODY()

public:
	void UpdateDirection(EFacingDirection Direction) override;
	bool IsMoveAnimationPlaying() const override;
	void StartMoveAnimation() override;
	bool CanStopMoving() override;
	void StopMoveAnimation() override;

	/**
	 * Get the flipbook component for this actor
	 * @return The flipbook component used
	 */
	UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = Character)
	UPaperFlipbookComponent *GetFlipbookComponent();
	
	/**
	 * Get the character set for the owning actor
	 * @return The character set for this actor
	 */
	UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = Character)
	UCharset* GetCharset() const;

	/**
	 * Set the character set for this component.
	 * @param NewCharset The character set to use.
	 */
	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, Category = Character)
	void SetCharset(UCharset* NewCharset);
	
private:

	/**
	 * Get the desired flipbook for the character
	 * @return Get the flipbook we want this character to have
	 */
	UPaperFlipbook* GetDesiredFlipbook() const;
	
	/**
	  * The flipbook component for this particular actor
	  */
	UPROPERTY(BlueprintGetter = GetFlipbookComponent, Category = Display)
	TObjectPtr<UPaperFlipbookComponent> FlipbookComponent;
	
	/**
	 * The character set used for displaying the sprite 
	 */
	UPROPERTY(EditAnywhere, BlueprintGetter = GetCharset, BlueprintSetter = SetCharset, Category = Character)
	TObjectPtr<UCharset> Charset;

	/**
	 * The direction this character is facing
	 */
	EFacingDirection CurrentDirection = EFacingDirection::Down;
};
