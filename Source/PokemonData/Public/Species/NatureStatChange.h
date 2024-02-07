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
#include "NatureStatChange.generated.h"

/**
 * Stores the stat change of a Nature
 */
USTRUCT(BlueprintType)
struct POKEMONDATA_API FNatureStatChange {
	GENERATED_BODY()

	/**
	 * The stat to alter
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FName Stat;

	/**
	 * The percentage to change the stat by. Positive means add that percentage to the stat. Negative means substract
	 * that percentage from the stat.
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (UIMin = -100, ClampMin = -100, UIMax = 100, ClampMax = 100))
	int32 Change;
};
