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
#include "FacingDirection.h"
#include "GameFramework/Actor.h"
#include "GameCharacter.generated.h"

class UPaperFlipbook;
class UCharset;
class UBoxComponent;
class UPaperFlipbookComponent;

/**
 * Basic character class used to represent a character moving in 2D space
 */
UCLASS(Blueprintable, ClassGroup=(Characters))
class GRIDBASED2D_API AGameCharacter : public AActor {
	GENERATED_BODY()

public:
	/**
	 * Sets default values for this actor's properties
	 */
	AGameCharacter();

protected:
	void BeginPlay() override;
	void PostInitProperties() override;
	void PostReinitProperties() override;
	void PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent);

public:
	void Tick(float DeltaTime) override;

private:
	/**
	 * Initialize the character sprite information based on the selected charset and direction
	 */
	void InitCharacterSpriteData();

	/**
	 * Get the desired flipbook for the character
	 * @return Get the flipbook we want this character to have
	 */
	UPaperFlipbook* GetDesiredFlipbook() const;
	
	/**
	 * The collider used to handle collisions for the character
	 */
	UPROPERTY()
	TObjectPtr<UBoxComponent> Collider;

	/**
	 * The actual sprite of the character in question
	 */
	UPROPERTY()
	TObjectPtr<UPaperFlipbookComponent> CharacterSprite;

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
};
