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
#include "Pokemon/Pokemon.h"

/**
 * Represents a the stat block for calculating the Pokémon's Stats
 */
class IStatBlock {
public:
	virtual ~IStatBlock() = default;

	/**
	 * Calculate the stat of the given Pokémon in question
	 * @param BaseStats The base stats of the Pokémon species/form
	 * @param Stat The stat to calculate
	 * @return The value of the stat
	 */
	virtual int32 CalculateStat(const TMap<FName, int32> &BaseStats, FName Stat) const;

	/**
	 * Get the level of the Pokémon in question
	 * @return The level of this particular Pokémon
	 */
	virtual int32 GetLevel() const;

	/**
	 * Get the Pokémon's IVs
	 * @return The full map of the Pokémon's IVs
	 */
	virtual TMap<FName, int32> &GetIVs();

	/**
	 * Get the IV for the given stat
	 * @return The full map of the Pokémon's IVs
	 */
	virtual const TMap<FName, int32> &GetIVs() const;

	/**
	 * Get the Pokémon's EVs
	 * @return The full map of the Pokémon's EVs
	 */
	virtual TMap<FName, int32> &GetEVs();
	
	/**
	 * Get the Pokémon's EVs
	 * @return The full map of the Pokémon's EVs
	 */
	virtual const TMap<FName, int32> &GetEVs() const;

	/**
	 * Get the Pokémon's Nature value
	 * @return The Nature of the Pokémon in question
	 */
	virtual const FNature &GetNature() const;
};
