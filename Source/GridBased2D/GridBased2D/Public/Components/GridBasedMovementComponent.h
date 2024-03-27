// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Characters/FacingDirection.h"
#include "Characters/MoveCheckResult.h"
#include "Components/ActorComponent.h"
#include "Map/WithinMap.h"
#include "GridBasedMovementComponent.generated.h"

class IGridBasedAnimationComponent;
class IInteractable;
class AGridBasedMap;

/**
 * Input action for when movement is complete
 */
DECLARE_DYNAMIC_DELEGATE(FMoveCompleteAction);

/**
 * Delegate called when the movement is completed
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMoveComplete);

/**
 * Component used to handle the movement of an actor along a fixed size grid.
 */
UCLASS(ClassGroup=(Movement), meta=(BlueprintSpawnableComponent))
class GRIDBASED2D_API UGridBasedMovementComponent : public UActorComponent, public IWithinMap {
	GENERATED_BODY()

public:
	/**
	 * Construct the default object
	 */
	UGridBasedMovementComponent();

protected:
	void BeginPlay() override;

public:
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/**
	 * Move the character in the specified direction
	 * @param MovementDirection The direction to move the character in
	 */
	UFUNCTION(BlueprintCallable, Category = "Character|Movement")
	void MoveInDirection(EFacingDirection MovementDirection);

	/**
	 * Move the character in the specified direction
	 * @param MovementDirection The direction to move the character in
	 * @param MovementCompleteCallback The functor to call when movement is completed
	 */
	void MoveInDirection(EFacingDirection MovementDirection, TFunction<void()> &&MovementCompleteCallback);

	/**
	 * Bind an action to the move complete action
	 * @param MoveCompleteAction The action to perform upon movement being finished
	 */
	UFUNCTION(BlueprintCallable, Category = Movement)
	void BindActionToOnMoveComplete(const FMoveCompleteAction& MoveCompleteAction);

	/**
	 * Check to see if the character can move in the specified direction
	 * @param MovementDirection The direction the character would like to move in
	 * @return Can the character move to that tile, as well and any interactable objects found when the check is done
	 */
	UFUNCTION(BlueprintPure, Category = "Character|Movement")
	FMoveCheckResult MovementCheck(EFacingDirection MovementDirection) const;

	/**
	 * Can this character move between the various maps of the overworld
	 * @return Can the character move between maps?
	 */
	bool CanMoveBetweenMaps() const;
	
	/**
	 * Turn the character to face a specific direction
	 * @param FacingDirection The direction to update the character's facing direction to
	 */
	UFUNCTION(BlueprintCallable, Category = "Character|Movement")
	void FaceDirection(EFacingDirection FacingDirection);

	/**
	 * Warp to the given location and offset
	 * @param X The X coordinate to warp to
	 * @param Y The Y coordinate to warp to
	 * @param Offset The physical offset of the map in the world
	 */
	UFUNCTION(BlueprintCallable, Category = "Character|Movement")
	void WarpToLocation(int32 X, int32 Y, FVector Offset) override;

	/**
	 * Called when the player changes maps
	 * @param NewMap The new map the player changes to
	 */
	void OnMapChanged(AGridBasedMap* NewMap);
	
	FIntVector2 GetCurrentPosition() const override;

	/**
	 * Get the position that this character is currently moving to
	 * @return The intended grid position of the character
	 */
	FIntVector2 GetDesiredPosition() const;

	/**
	 * Get the direction this character is actively facing
	 * @return The direction the character is facing.
	 */
	UFUNCTION(BlueprintPure, Category = "Character|Movement")
	EFacingDirection GetDirection() const;
	
	/**
	 * Perform a hit test on the tile in the given direction.
	 * @param MovementDirection The direction to check the file of
	 * @return Any hits that are found and all interactable actors on the tile
	 */
	TArray<FOverlapResult> HitTestOnFacingTile(EFacingDirection MovementDirection) const;

	/**
	 * Perform a hit interaction on all of the interactable objects in front of the player
	 * @param Interactables The interactable objects in question
	 */
	void HitInteraction(const TArray<TScriptInterface<IInteractable>>& Interactables);

private:
	/**
	 * Update the character's movement upon a tick
	 * @param DeltaTime The amount of time that has passed since the previous frame
	 */
	void UpdateMovement(float DeltaTime);

	/**
	 * Update the character's animation state
	 * @param DeltaTime The amount of time that has passed since the previous frame
	 */
	void UpdateAnimation(float DeltaTime);

	/**
	 * Called when the player completes their movement cycle
	 */
	void MoveComplete();

	/**
	 * The current position of the character within the movement grid
	 */
	UPROPERTY(VisibleAnywhere, Category = "Position")
	FIntVector2 CurrentPosition;

	/**
	 * The desired position of the character within the movement grid
	 */
	UPROPERTY(VisibleAnywhere, Category = "Position")
	FIntVector2 DesiredPosition;

	/**
	 * The direction this character is facing
	 */
	UPROPERTY(EditAnywhere, Category = "Character")
	EFacingDirection Direction = EFacingDirection::Down;

	/**
	 * Component for handling the animations of the character
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Character")
	TScriptInterface<IGridBasedAnimationComponent> GridBasedAnimationComponent;

	/**
	 * The timer for movement used to linearly interpolate the position to the new one
	 */
	TOptional<float> MoveTimer;

	/**
	 * One time callback for when movement is complete. 
	 */
	TOptional<TFunction<void()>> MoveCallback;

	/**
	 * Callback when the actor's movement is complete
	 */
	UPROPERTY(BlueprintAssignable, Category = Movement)
	FOnMoveComplete OnMoveComplete;
	
	/**
	 * The timer for movement used to determine where to stop animation
	 */
	TOptional<float> StopTimer;
	
};
