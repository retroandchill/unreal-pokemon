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
#include "DirectionalSprite.h"
#include "FacingDirection.h"
#include "UObject/Object.h"
#include "Charset.generated.h"

class UPaperFlipbook;

/**
 * Represents a 4 Directional Character sprite set for the game
 */
UCLASS(BlueprintType, HideCategories=(Object))
class GRIDBASED2D_API UCharset : public UObject {
	GENERATED_BODY()

public:
	/**
	 * Get the down facing sprite
	 * @return The Flipbook Asset for when the character is facing down
	 */
	UFUNCTION(BlueprintPure, Category = "Character|Sprites")
	UPaperFlipbook* GetDownSprite() const;

	/**
	 * Get the left facing sprite
	 * @return The Flipbook Asset for when the character is facing left
	 */
	UFUNCTION(BlueprintPure, Category = "Character|Sprites")
	UPaperFlipbook* GetLeftSprite() const;

	/**
	 * Get the right facing sprite
	 * @return The Flipbook Asset for when the character is facing right
	 */
	UFUNCTION(BlueprintPure, Category = "Character|Sprites")
	UPaperFlipbook* GetRightSprite() const;

	/**
	 * Get the up sprite
	 * @return The Flipbook Asset for when the character is facing up
	 */
	UFUNCTION(BlueprintPure, Category = "Character|Sprites")
	UPaperFlipbook* GetUpSprite() const;

	/**
	 * Get the character sprite for the given facing direction.
	 * @param Direction The direction to get the sprite for
	 * @return The Flipbook Asset for the given direction
	 */
	UFUNCTION(BlueprintPure, Category = "Character|Sprites")
	UPaperFlipbook* GetSprite(EFacingDirection Direction) const;

	/**
	 * Can the sprite stop moving on the provided frame if it is moving
	 * @param Direction The direction the character is facing
	 * @param CurrentFrame The current playback frame for the sprite
	 * @return If the character can safely stop
	 */
	UFUNCTION(BlueprintPure, Category = "Character|Sprites")
	bool CanStopOnFrame(EFacingDirection Direction, int32 CurrentFrame) const;

private:
	/**
	 * Get the full internal directional sprite data for the given direction
	 * @param Direction The direction to get the sprite for
	 * @return The Flibook Asset and set of valid stop frames
	 */
	const FDirectionalSprite* GetDirectionalSprite(EFacingDirection Direction) const;
	
	/**
	 * The Flipbook Asset for when the character is facing down
	 */
	UPROPERTY(EditAnywhere, Category = "Character|Sprites")
	FDirectionalSprite DownSprite;

	/**
	 * The Flipbook Asset for when the character is facing left
	 */
	UPROPERTY(EditAnywhere, Category = "Character|Sprites")
	FDirectionalSprite LeftSprite;

	/**
	 * The Flipbook Asset for when the character is facing left
	 */
	UPROPERTY(EditAnywhere, Category = "Character|Sprites")
	FDirectionalSprite RightSprite;

	/**
	 * The Flipbook Asset for when the character is facing up
	 */
	UPROPERTY(EditAnywhere, Category = "Character|Sprites")
	FDirectionalSprite UpSprite;
};
