//====================================================================================================================
// ** Unreal Pokémon created by Retro & Chill
//--------------------------------------------------------------------------------------------------------------------
// This project is intended as a means of learning more about how a game like Pokémon works by creating a framework
// from the ground up, and for non-commercial applications. While this code is original, Pokémon is the intellectual
// property of Game Freak and Nintendo, as such it is highly discouraged to use this kit to make a commercial product.
//--------------------------------------------------------------------------------------------------------------------
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//====================================================================================================================
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
	void Tick(float DeltaTime) override;
	
	void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

private:
	/**
	 * Perform the movement action when receiving the input
	 * @param Input The input information received
	 */
	void Move(const FInputActionInstance &Input);

	/**
	 * Perform the face direction action when receiving the input
	 * @param Input The input information received
	 */
	void Turn(const FInputActionInstance &Input);
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
