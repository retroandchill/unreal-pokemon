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
#include "Pokemon/GamePokemon.h"

#include "DataManager.h"
#include "Pokemon/Stats/DefaultStatBlock.h"

// TODO: Instantiate the stat block dynamically based on a user config
FGamePokemon::FGamePokemon(FName Species, int32 Level) : Species(Species) {
	auto &DataManager = FDataManager::GetInstance();
	auto &SpeciesTable = DataManager.GetDataTable<FSpeciesData>();

	auto SpeciesData = SpeciesTable.GetData(Species);
	check(SpeciesData != nullptr);
	StatBlock = MakeUnique<FDefaultStatBlock>(SpeciesData->GrowthRate, Level);
	StatBlock->CalculateStats(SpeciesData->BaseStats);
}

const FSpeciesData& FGamePokemon::GetSpecies() const {
	auto &DataTable = FDataManager::GetInstance().GetDataTable<FSpeciesData>();
	auto SpeciesData = DataTable.GetData(Species);
	check(SpeciesData != nullptr);

	return  *SpeciesData;
}

const IStatBlock& FGamePokemon::GetStatBlock() const {
	return *StatBlock;
}
