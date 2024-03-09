// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "GameCharacter.h"
#include "GamePlayer.generated.h"

class UAction;
class UInputAction;
class UCameraComponent;
class USpringArmComponent;
class UInputMappingContext;

/**
 * Character class to handle the player character
 */
UCLASS(Blueprintable, ClassGroup=(Characters))
class GRIDBASED2D_API AGamePlayer : public AGameCharacter {
	GENERATED_BODY()

public:
	/**
	 * Sets default values for this pawn's properties
	 */
	AGamePlayer();

protected:
	void BeginPlay() override;

public:
	void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;

	/**
	 * The orthographic camera used to display the world to the player
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> TopDownCamera;

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
};
