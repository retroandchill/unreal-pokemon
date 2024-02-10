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
#include "StatEntry.h"
#include "Species/Nature.h"

/**
 * Represents a the stat block for calculating the Pokémon's Stats
 */
class POKEMONCORE_API IStatBlock {
public:
	virtual ~IStatBlock() = default;

	/**
	 * Get the level of the Pokémon in question
	 * @return The level of this particular Pokémon
	 */
	virtual int32 GetLevel() const = 0;

	/**
	 * Get the current amount of Exp. this Pokémon has
	 * @return The current amount of Exp.
	 */
	virtual int32 GetExp() const = 0;

	/**
	 * Get the amount of Exp. required to reach the next level
	 * @return The exp amount required to reach the next level
	 */
	virtual int32 GetExpForNextLevel() const = 0;

	/**
	 * Get the Pokémon's Nature value
	 * @return The Nature of the Pokémon in question
	 */
	virtual const FNature &GetNature() const = 0;

	/**
	 * Get the stat that corresponds to the given ID
	 * @param Stat The stat ID to retrieve
	 * @return The entry of the stat
	 */
	virtual IStatEntry &GetStat(FName Stat) = 0;

	/**
	 * Get the stat that corresponds to the given ID
	 * @param Stat The stat ID to retrieve
	 * @return The entry of the stat
	 */
	virtual const IStatEntry &GetStat(FName Stat) const = 0;

	/**
	 * Calculate the stats of the given Pokémon in question
	 * @param BaseStats The base stats of the Pokémon species/form
	 */
	virtual void CalculateStats(const TMap<FName, int32> &BaseStats) = 0;
	
};
