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
#include "UObject/Object.h"
#include "LevelUpMove.generated.h"

/**
 * Represents a move learned upon level up
 */
USTRUCT(BlueprintType)
struct POKEMONDATA_API FLevelUpMove {
	GENERATED_BODY()

	/**
	 * The level this move is learned at (0 = learned upon evolution)
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Moves", meta = (UIMin = 0, ClampMin = 0))
	int Level = 1;

	/**
	 * The ID of the move that is learned at that level
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Moves")
	FName Move;
};
