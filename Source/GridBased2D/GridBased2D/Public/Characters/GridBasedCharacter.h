﻿// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Components/GridMovable.h"
#include "GameFramework/Character.h"
#include "GameFramework/Pawn.h"
#include "Interaction/Interactable.h"
#include "GridBasedCharacter.generated.h"

class UGridBasedMovementComponent;
class UAction;
class UInputAction;
class UInputMappingContext;
struct FInputActionInstance;

/**
 * Abstract pawn declaration for a grid-based character
 */
UCLASS(Abstract)
class GRIDBASED2D_API AGridBasedCharacter : public ACharacter, public IGridMovable, public IInteractable {
	GENERATED_BODY()

public:
	/**
	 * Initialize the default object
	 */
	AGridBasedCharacter();

protected:
	void BeginPlay() override;

public:
	void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	UGridBasedMovementComponent* GetGridBasedMovementComponent_Implementation() const override;

	void OnInteract_Implementation(const TScriptInterface<IGridMovable>& Character, EInteractionType InteractionType) override;
	bool PerformAdditionalMovementChecks_Implementation(const FVector& TargetSquare, bool bBlockingHit, UPrimitiveComponent* HitComponent) override;
	
	UFUNCTION(BlueprintPure, Category = "Interaction")
	uint8 GetInteractionTypes() const override;

protected:
	/**
	 * Internal method that sidesteps the interface event to get the movement component
	 * @return The retrieve movement component
	 */
	UGridBasedMovementComponent* GetGridBasedMovementComponentInternal() const;

	/**
	 * Check if a tile is a valid floor tile
	 * @param TargetSquare The world space location of the tile in question
	 * @param HitComponent The actor component that was hit in the collision
	 * @return Is there no valid floor at this height
	 */
	UFUNCTION(BlueprintPure, Category = "Map|Grid")
	bool InvalidFloor(const FVector &TargetSquare, const UPrimitiveComponent* HitComponent);

	/**
	 * Check if a tile is a cliff face
	 * @param TargetSquare The world space location of the tile in question
	 * @return Is the target tile a cliff
	 */
	UFUNCTION(BlueprintPure, Category = "Map|Grid")
	bool IsStandingNextToCliff(const FVector& TargetSquare);

	/**
	 * Checks if the character can step up on the selected component at all
	 * @param Component The component to check
	 * @return Can the player step up?
	 */
	bool CanStepUpOnComponent(const UPrimitiveComponent& Component);

	/**
	 * Find the world space position just off of a tile's edge
	 * @param TargetSquare The world space location of the tile in question
	 * @return What is the world space location of one pixel off of the player's current tile
	 */
	FVector FindLocationJustOffTileEdge(const FVector &TargetSquare) const;

	/**
	 * Compute the distance to the ground from the current position
	 * @param Position The position to sweep to the ground from
	 * @return The distance to the ground and the hit component
	 */
	TPair<double, UPrimitiveComponent*> PerformTraceToGround(const FVector &Position) const;

private:
	/**
	 * Perform the movement action when receiving the input
	 * @param Input The input information received
	 */
	void Move(const FInputActionInstance& Input);

	/**
	 * Perform the face direction action when receiving the input
	 * @param Input The input information received
	 */
	void Turn(const FInputActionInstance& Input);

	/**
	 * Interact with the object directly in front of the player;
	 */
	void Interact();

	/**
	 * Pause the game and pull up the menu.
	 */
	void PauseGame();

	/**
	 * The boom arm for holding the camera.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UGridBasedMovementComponent> GridBasedMovementComponent;

	/**
	 * The mapping context asset to use to determine movement controls
	 */
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	/**
	 * The input to use for movement on the map
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TSoftObjectPtr<UInputAction> MoveInput;

	/**
	 * The input to use for facing a given direction
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TSoftObjectPtr<UInputAction> FaceDirectionInput;

	/**
	 * The input used for interacting with other objects
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TSoftObjectPtr<UInputAction> InteractInput;

	/**
	 * The input to use for pausing the game
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TSoftObjectPtr<UInputAction> PauseInput;

	/**
	 * The class for the action that is invoked when the player pauses the game
	 */
	UPROPERTY(EditDefaultsOnly, Instanced, Category = Actions)
	TObjectPtr<UAction> PauseAction;

	/**
	 * Called when the player interacts with this object
	 */
	UPROPERTY(BlueprintAssignable, Category = Interaction)
	FOnPlayerInteraction OnInteractedWith;

	/**
	 * The list of valid interaction types
	 */
	UPROPERTY(EditAnywhere, Category = Interaction, meta = (Bitmask, BitmaskEnum = EInteractionType))
	uint8 InteractionTypes = static_cast<uint8>(EInteractionType::Talk);
};
