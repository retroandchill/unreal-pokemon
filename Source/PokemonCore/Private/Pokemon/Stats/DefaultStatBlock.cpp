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
#include "Pokemon/Stats/DefaultMainBattleStatEntry.h"
#include "Pokemon/Stats/DefaultMainStatEntry.h"
#include "Pokemon/Stats/StatUtils.h"
#include "Species/Stat.h"

using namespace StatUtils;

FDefaultStatBlock::~FDefaultStatBlock() = default;

FDefaultStatBlock::FDefaultStatBlock(const FDefaultStatBlock& Other) : Level(Other.Level), Nature(Other.Nature) {
	for (auto &[StatID, Stat] : Other.Stats) {
		Stats.Add(StatID, Stat->Clone());
	}
}

FDefaultStatBlock::FDefaultStatBlock(FDefaultStatBlock&& Other) noexcept = default;

FDefaultStatBlock& FDefaultStatBlock::operator=(const FDefaultStatBlock& Other) {
	Level = Other.Level;
	Nature = Other.Nature;
	
	Stats.Empty(Other.Stats.Num());
	for (auto &[StatID, Stat] : Other.Stats) {
		Stats.Add(StatID, Stat->Clone());
	}

	return *this;
}

FDefaultStatBlock& FDefaultStatBlock::operator=(FDefaultStatBlock&& Other) noexcept = default;

FDefaultStatBlock::FDefaultStatBlock(int32 Level) : Level(Level), Nature(RandomNature()) {
	auto &DataSubsystem = FDataManager::GetInstance();
	auto &StatTable = DataSubsystem.GetDataTable<FStat>();
	
	StatTable.ForEach([this](const FStat &Stat) {
		switch (Stat.Type) {
		case EPokemonStatType::Main:
			Stats.Add(Stat.ID, MakeUnique<FDefaultMainStatEntry>(Stat.ID, FMath::RandRange(0, 31)));
			break;
		case EPokemonStatType::MainBattle:
			Stats.Add(Stat.ID, MakeUnique<FDefaultMainBattleStatEntry>(Stat.ID, FMath::RandRange(0, 31)));
			break;
		case EPokemonStatType::Battle:
			// Skip over this stat as we don't track a value for it
			break;
		}
	});
}

FDefaultStatBlock::FDefaultStatBlock(int32 Level, const TMap<FName, int32>& IVs,
	const TMap<FName, int32>& EVs, FName Nature) : Level(Level), Nature(Nature) {
	auto &DataSubsystem = FDataManager::GetInstance();
	auto &StatTable = DataSubsystem.GetDataTable<FStat>();
	
	StatTable.ForEach([this, &IVs, &EVs](const FStat &Stat) {
		switch (Stat.Type) {
		case EPokemonStatType::Main:
			check(IVs.Contains(Stat.ID) && EVs.Contains(Stat.ID));
			Stats.Add(Stat.ID, MakeUnique<FDefaultMainStatEntry>(Stat.ID, IVs[Stat.ID], EVs[Stat.ID]));
			break;
		case EPokemonStatType::MainBattle:
			check(IVs.Contains(Stat.ID) && EVs.Contains(Stat.ID));
			Stats.Add(Stat.ID, MakeUnique<FDefaultMainBattleStatEntry>(Stat.ID, IVs[Stat.ID], EVs[Stat.ID]));
			break;
		case EPokemonStatType::Battle:
			// Skip over this stat as we don't track a value for it
				break;
			}
		});
}

int32 FDefaultStatBlock::GetLevel() const {
	return Level;
}

const FNature& FDefaultStatBlock::GetNature() const {
	auto &DataSubsystem = FDataManager::GetInstance();
	auto &NatureTable = DataSubsystem.GetDataTable<FNature>();

	auto Ret = NatureTable.GetData(Nature);
	check(Ret != nullptr);
	return *Ret;
}

IStatEntry& FDefaultStatBlock::GetStat(FName Stat) {
	check(Stats.Contains(Stat));
	return *Stats[Stat];
}

const IStatEntry& FDefaultStatBlock::GetStat(FName Stat) const {
	check(Stats.Contains(Stat));
	return *Stats[Stat];
}

void FDefaultStatBlock::CalculateStats(const TMap<FName, int32>& BaseStats) {
	auto &NatureData = GetNature();
	
	for (auto &[StatID, Stat] : Stats) {
		check(BaseStats.Contains(StatID));
		Stat->RefreshValue(Level, BaseStats[StatID], NatureData);
	}
}
