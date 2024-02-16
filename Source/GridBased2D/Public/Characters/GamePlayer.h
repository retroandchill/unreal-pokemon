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

class UInputAction;
class UCameraComponent;
class USpringArmComponent;
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
	
	void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	/**
	 * Perform the movement action when receiving the input
	 * @param Input The input information received
	 */
	void Move(const FInputActionInstance &Input);
	
	/**
	 * The boom arm for holding the camera.
	 */
	UPROPERTY()
	TObjectPtr<USpringArmComponent> CameraBoom;

	/**
	 * The orthographic camera used to display the world to the player
	 */
	UPROPERTY()
	TObjectPtr<UCameraComponent> TopDownCamera;

	/**
	 * The input to use for movement on the map
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TSoftObjectPtr<UInputAction> MoveInput;
	
};
