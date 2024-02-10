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
#include "Pokemon/Exp/GrowthRate.h"

namespace StatUtils {
	/**
	 * Randomize the Pokémon's IVs and return a map of the values
	 * @return The randomized IVs
	 */
	POKEMONCORE_API TMap<FName, int32> RandomizeIVs();

	/**
	 * Set all of the EVs to 0
	 * @return The map of EVs all set to 0
	 */
	POKEMONCORE_API TMap<FName, int32> DefaultEVs();

	/**
	 * Generate a random nature for the Pokémon
	 * @return The randomized Nature for the Pokémon in question
	 */
	POKEMONCORE_API FName RandomNature();

	/**
	 * Create a new growth rate instance with the provided key
	 * @param GrowthRate The name of the growth rate to get the ID for
	 * @return The created growth rate
	 */
	POKEMONCORE_API TUniquePtr<Exp::IGrowthRate> CreateGrowthRate(FName GrowthRate);

	/**
	 * Get the maximum achievable level for a Pokémon
	 */
	POKEMONCORE_API int32 GetMaxLevel();
}

