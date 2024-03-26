// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "GridBasedAnimationComponent.h"
#include "Components/ActorComponent.h"
#include "CharsetComponent.generated.h"

class UPaperFlipbook;
class UCharset;
class UPaperFlipbookComponent;
/**
 * Component used to manage charset animations
 */
UCLASS(ClassGroup=(Characters), meta=(BlueprintSpawnableComponent))
class GRIDBASED2D_API UCharsetComponent : public UActorComponent, public IGridBasedAnimationComponent {
	GENERATED_BODY()

protected:
	void BeginPlay() override;

public:
	void UpdateDirection(EFacingDirection Direction) override;
	bool IsMoveAnimationPlaying() const override;
	void StartMoveAnimation() override;
	bool CanStopMoving() override;
	void StopMoveAnimation() override;
	
private:

	/**
	 * Get the desired flipbook for the character
	 * @return Get the flipbook we want this character to have
	 */
	UPaperFlipbook* GetDesiredFlipbook() const;
	
	/**
	  * The flipbook component for this particular actor
	  */
	UPROPERTY()
	TObjectPtr<UPaperFlipbookComponent> FlipbookComponent;
	
	/**
	 * The character set used for displaying the sprite 
	 */
	UPROPERTY(EditAnywhere, Category = Character)
	TObjectPtr<UCharset> Charset;

	/**
	 * The direction this character is facing
	 */
	EFacingDirection CurrentDirection = EFacingDirection::Down;
};
