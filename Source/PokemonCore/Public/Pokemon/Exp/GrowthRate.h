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
#include "Meta/GenericStaticRegistry.h"


namespace Exp {
	/**
	 * Interface to represent various Exp. grow rates
	 */
	class POKEMONCORE_API IGrowthRate {

	public:
		virtual ~IGrowthRate() = default;
	
		/**
		 * Gets the amount of Exp that is required to reach a certain level 
		 * @param Level The level in to calculate the Exp. for
		 * @return The amount of Exp. required to level up
		 */
		virtual int32 ExpForLevel(int32 Level) const = 0;

		/**
		 * Create a duplicate of this object
		 * @return A unique reference to a deep copy of this object
		 */
		virtual TUniquePtr<IGrowthRate> Clone() const = 0;
	};

	/**
	 * Static registry for the growth rate types
	 */
	using FGrowthRateRegistry = TGenericStaticRegistry<IGrowthRate>;
};
