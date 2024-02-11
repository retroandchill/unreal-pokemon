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
#include "DefaultStatEntry.h"

/**
 * Handles the default representation of the main battle stats
 */
class POKEMONCORE_API FDefaultMainBattleStatEntry : public FDefaultStatEntry {
public:
	/**
	 * Initialize the stat with the given IV and EV
	 * @param Stat The stat in question to set this to
	 * @param IV The IV of the stat
	 * @param EV The EV of the stat
	 */
	FDefaultMainBattleStatEntry(FName Stat, int32 IV, int32 EV = 0);
	
	void RefreshValue(int32 Level, int32 Base, const FNature& Nature) final;
	TUniquePtr<IStatEntry> Clone() const override;
};
