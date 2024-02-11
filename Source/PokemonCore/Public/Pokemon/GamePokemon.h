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
#include "Pokemon.h"
#include "Stats/StatBlock.h"

/**
 * Basic Pokémon class that holds all of the information for a complete Pokémon
 */
class POKEMONCORE_API FGamePokemon : public IPokemon {
public:
	/**
	 * Create a Pokémon with the following species and level information
	 * @param Species The species of Pokémon to create
	 * @param Level The level of the Pokémon in question
	 */
	explicit FGamePokemon(FName Species, int32 Level = 5);
	
	const FSpeciesData& GetSpecies() const override;
	const IStatBlock& GetStatBlock() const override;

private:
	/**
	 * The ID of the species of Pokémon this is
	 */
	FName Species;

	TUniquePtr<IStatBlock> StatBlock;
};
