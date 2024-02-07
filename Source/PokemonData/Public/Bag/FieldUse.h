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
#include "FieldUse.generated.h"

/**
 * Represents the different options for how an item can be used in the field
 */
UENUM(BlueprintType)
enum class EFieldUse : uint8 {
	/**
	 * Not usable in the field
	 */
	NoFieldUse = 0,

	/**
	 * Used on a Pokémon
	 */
	OnPokemon = 1,

	/**
	 * Used directly from the bag
	 */
	Direct = 2,

	/**
	 * Teaches a Pokémon a move (resuable on newer mechanics)
	 */
	TM = 3,

	/**
	 * Teaches a Pokémon a move (reusable)
	 */
	HM = 4,

	/**
	 * Teaches a Pokémon a move (single-use)
	 */
	TR = 5
};
