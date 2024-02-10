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
#include "StatBlock.h"

/**
 * The default stat calculator, which uses the formulas from the main series games
 */
class POKEMONCORE_API FDefaultStatBlock final : public IStatBlock {
public:
	/**
	 * Initialize a new stat block with the given level randomizing the IVs and nature
	 * @param Level The level to start at
	 */
	explicit FDefaultStatBlock(int32 Level = 5);

	/**
	 * Initialize a new stat block explicitly setting the values of all the Pokémon's stats
	 * @param Level The level to start at 
	 * @param IVs The values of the IVs
	 * @param EVs The values of the EVs
	 * @param Nature The nature of the Pokémon in question
	 */
	FDefaultStatBlock(int32 Level, const TMap<FName, int32> &IVs, const TMap<FName, int32> &EVs, FName Nature);

	/**
	 * Initialize a new stat block explicitly setting the values of all the Pokémon's stats
	 * @param Level The level to start at 
	 * @param IVs The values of the IVs
	 * @param EVs The values of the EVs
	 * @param Nature The nature of the Pokémon in question
	 */
	FDefaultStatBlock(int32 Level, TMap<FName, int32> &&IVs, TMap<FName, int32> &&EVs, FName Nature);
	
	int32 CalculateStat(const TMap<FName, int32>& BaseStats, FName Stat) const override;
	int32 GetLevel() const override;
	TMap<FName, int32>& GetIVs() override;
	const TMap<FName, int32>& GetIVs() const override;
	TMap<FName, int32>& GetEVs() override;
	const TMap<FName, int32>& GetEVs() const override;
	const FNature& GetNature() const override;

private:
	/**
	 * The level of the Pokémon
	 */
	int32 Level;

	/**
	 * The Pokémon's IVs
	 */
	TMap<FName, int32> IVs;

	/**
	 * The Pokémon's EVs
	 */
	TMap<FName, int32> EVs;

	/**
	 * The Pokémon's Nature ID
	 */
	FName Nature;
};
