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
#include "Pokemon/Stats/DefaultStatBlock.h"

#include "DataManager.h"
#include "Pokemon/Stats/StatUtils.h"
#include "Species/Stat.h"

using namespace StatUtils;

FDefaultStatBlock::FDefaultStatBlock(int32 Level) : Level(Level), IVs(RandomizeIVs()), EVs(DefaultEVs()), Nature(RandomNature()) {
}

FDefaultStatBlock::FDefaultStatBlock(int32 Level, const TMap<FName, int32>& IVs,
	const TMap<FName, int32>& EVs, FName Nature) : Level(Level), IVs(IVs), EVs(EVs), Nature(Nature) {
}

FDefaultStatBlock::FDefaultStatBlock(int32 Level, TMap<FName, int32>&& IVs, TMap<FName, int32>&& EVs,
	FName Nature) : Level(Level), IVs(MoveTemp(IVs)), EVs(MoveTemp(EVs)), Nature(Nature) {
}

int32 FDefaultStatBlock::CalculateStat(const TMap<FName, int32>& BaseStats, FName Stat) const {
	auto &DataSubsystem = FDataManager::GetInstance();
	
	auto StatData = DataSubsystem.GetDataTable<FStat>().GetData(Stat);
	check(StatData != nullptr);
	check(StatData->Type != EPokemonStatType::Battle);
	
	check(BaseStats.Contains(Stat));
	if (StatData->Type == EPokemonStatType::Main) {
		return (2 * BaseStats[Stat] + IVs[Stat] + EVs[Stat] / 4) * Level / 100 + Level + 10;
	}
	
	auto NatureData = DataSubsystem.GetDataTable<FNature>().GetData(Nature);
	check(NatureData != nullptr);
		
	auto NatureChange = NatureData->StatChanges.FindByPredicate([&Stat](const FNatureStatChange &Change) {
		return Change.Stat == Stat;
	});
	int32 NatureModifer = NatureChange != nullptr ? 100 + NatureChange->Change : 100;
		
	return ((2 * BaseStats[Stat] + IVs[Stat] + EVs[Stat] / 4) * Level / 100 + 5) * NatureModifer / 100;
}

int32 FDefaultStatBlock::GetLevel() const {
	return Level;
}

TMap<FName, int32>& FDefaultStatBlock::GetIVs() {
	return IVs;
}

const TMap<FName, int32>& FDefaultStatBlock::GetIVs() const {
	return IVs;
}

TMap<FName, int32>& FDefaultStatBlock::GetEVs() {
	return EVs;
}

const TMap<FName, int32>& FDefaultStatBlock::GetEVs() const {
	return EVs;
}

const FNature& FDefaultStatBlock::GetNature() const {
	auto &DataSubsystem = FDataManager::GetInstance();
	auto &NatureTable = DataSubsystem.GetDataTable<FNature>();

	auto Ret = NatureTable.GetData(Nature);
	check(Ret != nullptr);
	return *Ret;
}
