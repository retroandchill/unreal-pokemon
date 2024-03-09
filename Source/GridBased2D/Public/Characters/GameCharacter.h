// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "FacingDirection.h"
#include "PaperCharacter.h"
#include "GameCharacter.generated.h"

struct FInputActionInstance;
class UPaperFlipbook;
class UCharset;
class UBoxComponent;
class UPaperFlipbookComponent;

/**
 * Basic character class used to represent a character moving in 2D space
 */
UCLASS(Blueprintable, ClassGroup=(Characters))
class GRIDBASED2D_API AGameCharacter : public APaperCharacter {
	GENERATED_BODY()

public:
	/**
	 * Sets default values for this actor's properties
	 */
	AGameCharacter();

protected:
	void PostInitProperties() override;
	void PostReinitProperties() override;
	void PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent) override;
	void PostLoad() override;
	void PostEditMove(bool bFinished) override;
	
	void BeginPlay() override;

public:
	void Tick(float DeltaTime) override;

	/**
	 * Move the character in the specified direction
	 * @param MovementDirection The direction to move the character in
	 */
	UFUNCTION(BlueprintCallable, Category = "Character|Movement")
	void MoveInDirection(EFacingDirection MovementDirection);

	/**
	 * Check to see if the character can move in the specified direction
	 * @param MovementDirection The direction the character would like to move in
	 * @return Can the character move to that tile
	 */
	UFUNCTION(BlueprintPure, Category = "Character|Movement")
	bool CanMoveInDirection(EFacingDirection MovementDirection) const;

	/**
	 * Turn the character to face a specific direction
	 * @param FacingDirection The direction to update the character's facing direction to
	 */
	UFUNCTION(BlueprintCallable, Category = "Character|Movement")
	void FaceDirection(EFacingDirection FacingDirection);

protected:
	FHitResult HitTestOnFacingTile(EFacingDirection MovementDirection) const;

private:
	/**
	 * Initialize the character sprite information based on the selected charset and direction
	 */
	void InitCharacterData();

	/**
	 * Get the desired flipbook for the character
	 * @return Get the flipbook we want this character to have
	 */
	UPaperFlipbook* GetDesiredFlipbook() const;

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

protected:
	/**
	 * Set the charset sprite to the specified value
	 * @param NewCharset The Charset asset to set this character to
	 */
	void SetCharset(UCharset* NewCharset);

public:
	/**
	 * Get the current grid position of this character
	 * @return The current grid position of the character
	 */
	FIntVector2 GetCurrentPosition() const;

	/**
	 * Get the position that this character is currently moving to
	 * @return The intended grid position of the character
	 */
	FIntVector2 GetDesiredPosition() const;

	/**
	 * Get the direction this character is actively facing
	 * @return The direction the character is facing.
	 */
	EFacingDirection GetDirection() const;

private:

	/**
	 * The character set used for displaying the sprite 
	 */
	UPROPERTY(EditAnywhere, Category = "Character")
	TObjectPtr<UCharset> Charset;
	
	/**
	 * The direction this character is facing
	 */
	UPROPERTY(EditAnywhere, Category = "Character")
	EFacingDirection Direction = EFacingDirection::Down;

	/**
	 * Can other characters step onto the same tile as this character?
	 */
	UPROPERTY(EditAnywhere, Category = "Collision")
	bool bPassThrough = false;

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
	 * The timer for movement used to linearly interpolate the position to the new one
	 */
	TOptional<float> MoveTimer;

	/**
	 * The timer for movement used to determine where to stop animation
	 */
	TOptional<float> StopTimer;
};
