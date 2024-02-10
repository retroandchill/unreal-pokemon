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
#include "Species/Nature.h"
#include "Species/Stat.h"

/**
 * Represents an individual stat entry on a Pokémon
 */
class POKEMONCORE_API IStatEntry {
public:
	virtual ~IStatEntry() = default;

	/**
	 * Get the value of this given stat in question
	 * @return The value of the stat
	 */
	virtual int32 GetStatValue() const = 0;

	/**
	 * Recalculate the value of the stat given the following information
	 * @param Level The level of the Pokémon
	 * @param Base The base stat of the Pokémon in question
	 * @param Nature The Pokémon's effective nature
	 */
	virtual void RefreshValue(int32 Level, int32 Base, const FNature &Nature) = 0;

	/**
	 * Get the information about the stat in question
	 * @return The stat's specific information
	 */
	virtual const FStat &GetStat() const = 0;

	/**
	 * Get the ID of the stat in question
	 * @return The ID of the stat in question
	 */
	virtual FName GetStatID() const = 0;

	/**
	 * Get the IV of the stat
	 * @return The value of the Pokémon's IV
	 */
	virtual int32 GetIV() const = 0;

	/**
	 * Get the EV of the stat
	 * @return The value of the Pokémon's EV
	 */
	virtual int32 GetEV() const = 0;

	/**
	 * Create a unique copy of this object
	 * @return A unique copy of this object
	 */
	virtual TUniquePtr<IStatEntry> Clone() const = 0;
};
