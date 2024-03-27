// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Characters/FacingDirection.h"
#include "UObject/Interface.h"
#include "GridBasedAnimationComponent.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UGridBasedAnimationComponent : public UInterface {
	GENERATED_BODY()
};

/**
 * Interface for components that facilitate animations within a grid based movement setup
 */
class GRIDBASED2D_API IGridBasedAnimationComponent {
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/**
	 * Update the character's animation based on the current direction
	 * @param Direction The direction the character is currently facing
	 */
	virtual void UpdateDirection(EFacingDirection Direction) = 0;
	
	/**
	 * Check if the character's movement animation is actually playing or not
	 * @return If the move animation is currently playing
	 */
	virtual bool IsMoveAnimationPlaying() = 0;

	/**
	 * Start playing the character's movement animation
	 */
	virtual void StartMoveAnimation() = 0;

	/**
	 * Is the character's animation in a state where it can stop moving and it look natural?
	 * @return If the character can stop the movement animation
	 */
	virtual bool CanStopMoving() = 0;

	/**
	 * Stop playing the character's movement animation
	 */
	virtual void StopMoveAnimation() = 0;
};
