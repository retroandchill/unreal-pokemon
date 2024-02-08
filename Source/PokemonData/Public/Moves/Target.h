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
#include "TargetCount.h"
#include "UObject/Object.h"
#include "Target.generated.h"

/**
 * Handles target processing for a move
 */
USTRUCT(BlueprintType, meta = (DatabaseType="Hardcoded"))
struct POKEMONDATA_API FTarget : public FTableRowBase {
	GENERATED_BODY()

	/**
	 * The internal ID used for lookup by the game
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Basic")
	FName ID;

	/**
	 * The name that is displayed to the player
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Basic")
	FText RealName;

	/**
	 * How many targets does this move have? 
	 */
	UPROPERTY(BlueprintType, EditAnywhere, Category = "Target Selection")
	ETargetCount NumTargets;

	/**
	 * Does this move target a Pokémon on the opposing side?
	 */
	UPROPERTY(BlueprintType, EditAnywhere, Category = "Target Selection")
	bool TargetsFoe = false;

	/**
	 * Does this move target both sides of the field?
	 */
	UPROPERTY(BlueprintType, EditAnywhere, Category = "Target Selection")
	bool TargetsAll = false;

	/**
	 * Does this move affect the opposing side of the field?
	 */
	UPROPERTY(BlueprintType, EditAnywhere, Category = "Target Selection")
	bool AffectsFoeSide = false;

	/**
	 * Is this move able to hit an opponent that is not directly adjacent to the target?
	 */
	UPROPERTY(BlueprintType, EditAnywhere, Category = "Target Selection")
	bool LongRange = false;
};
