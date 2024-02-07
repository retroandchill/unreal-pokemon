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
#include "PokemonStatType.h"
#include "UObject/Object.h"
#include "Stat.generated.h"

/**
 * Represents one of the stats in the database
 */
USTRUCT(BlueprintType, meta = (DatabaseType="Hardcoded"))
struct POKEMONDATA_API FStat : public FTableRowBase {
	GENERATED_BODY()

	/**
	 * The internal ID used for lookup by the game
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FName ID;

	/**
	 * The name that is displayed to the player
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FText RealName;

	/**
	 * The name that is displayed to the player when there is a limited space
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FText RealNameBrief;

	/**
	 * What kind of stat is this?
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	EPokemonStatType Type;

	/**
	 * The order this stat is processed when parsing the PBS files
	 */
	UPROPERTY(DisplayName = "PBS Order", BlueprintReadOnly, EditAnywhere, meta = (EditCondition = "Type != EPokemonStatType::Battle", UIMin = 0, ClampMin = 0))
	int32 PbsOrder;
};
