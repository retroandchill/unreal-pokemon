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
#include "BattleUse.generated.h"

/**
 * Represents the different options for how an item can be used in battle
 */
UENUM(BlueprintType)
enum class EBattleUse : uint8 {
	/**
	 * Not usable in battle
	 */
	NoBattleUse = 0,

	/**
	 * Usable on a Pokémon in the party
	 */
	OnPokemon = 1,

	/**
	 * Usable on a Pokémon in the party and requiring a move to be selected
	 */
	OnMove = 2,

	/**
	 * Usable on the active Pokémon in battle
	 */
	OnBattler = 3,

	/**
	 * Used on an opponent in battle
	 */
	OnFoe = 4,

	/**
	 * Used directly with no target selection
	 */
	Direct = 5
};
