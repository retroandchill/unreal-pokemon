// "Unreal Pokémon" created by Retro & Chill.
#include "Pokemon/Stats/DefaultStatBlock.h"

#include "DataManager.h"
#include "Pokemon/Stats/DefaultMainBattleStatEntry.h"
#include "Pokemon/Stats/DefaultMainStatEntry.h"
#include "Pokemon/Stats/StatBlockDTO.h"
#include "Pokemon/Stats/StatUtils.h"
#include "Species/Stat.h"

using namespace StatUtils;

/**
 * Helper function to find a nature by the given ID
 * @param Nature The nature to look up
 * @return The found nature
 */
TRowPointer<FNature> FindNature(FName Nature) {
	const auto& DataSubsystem = FDataManager::GetInstance();
	auto& NatureTable = DataSubsystem.GetDataTable<FNature>();

	auto Ret = NatureTable.GetDataManaged(Nature);
	check(Ret != nullptr)
	return Ret;
}

/**
 * Helper function to find a nature by the given DTO
 * @param DTO The DTO to get the nature from
 * @return The found nature
 */
TOptional<TRowPointer<FNature>> FindNature(const FStatBlockDTO& DTO) {
	return DTO.bOverride_Nature ? TOptional(FindNature(DTO.Nature)) : TOptional<TRowPointer<FNature>>();
}

FDefaultStatBlock::FDefaultStatBlock(FName GrowthRateID, uint32 PersonalityValue, const FStatBlockDTO& DTO) : Level(DTO.Level),
	PersonalityValue(PersonalityValue), GrowthRate(CreateGrowthRate(GrowthRateID)), Exp(GrowthRate->ExpForLevel(Level)),
	Nature(FindNature(DTO)) {
	auto& DataSubsystem = FDataManager::GetInstance();
	auto& StatTable = DataSubsystem.GetDataTable<FStat>();

	StatTable.ForEach([this, PersonalityValue, &DTO](const FStat& Stat) {
		auto IV = DTO.IVs.Contains(Stat.ID) ? TOptional<int32>(DTO.IVs[Stat.ID]) : TOptional<int32>();
		int32 EV = DTO.EVs.Contains(Stat.ID) ? DTO.EVs[Stat.ID] : 0;
		switch (Stat.Type) {
			using enum EPokemonStatType;
		case Main:
			Stats.Add(Stat.ID, MakeUnique<FDefaultMainStatEntry>(Stat.ID, PersonalityValue, IV, EV));
			break;
		case MainBattle:
			Stats.Add(Stat.ID, MakeUnique<FDefaultMainBattleStatEntry>(Stat.ID, PersonalityValue, IV, EV));
			break;
		case Battle:
			// Skip over this stat as we don't track a value for it
				break;
			}
		});
}

FDefaultStatBlock::~FDefaultStatBlock() = default;

FDefaultStatBlock::FDefaultStatBlock(const FDefaultStatBlock& Other) : Level(Other.Level),
                                                                       GrowthRate(Other.GrowthRate->Clone()),
                                                                       Exp(FMath::Max(
	                                                                       Other.Exp, GrowthRate->ExpForLevel(Level))),
                                                                       Nature(Other.Nature) {
	for (auto& [StatID, Stat] : Other.Stats) {
		Stats.Add(StatID, Stat->Clone());
	}
}

FDefaultStatBlock::FDefaultStatBlock(FDefaultStatBlock&& Other) noexcept = default;

FDefaultStatBlock& FDefaultStatBlock::operator=(const FDefaultStatBlock& Other) {
	Level = Other.Level;
	Nature = Other.Nature;
	GrowthRate = Other.GrowthRate->Clone();
	Exp = FMath::Max(Other.Exp, GrowthRate->ExpForLevel(Level));

	Stats.Empty(Other.Stats.Num());
	for (auto& [StatID, Stat] : Other.Stats) {
		Stats.Add(StatID, Stat->Clone());
	}

	return *this;
}

FDefaultStatBlock& FDefaultStatBlock::operator=(FDefaultStatBlock&& Other) noexcept = default;

int32 FDefaultStatBlock::GetLevel() const {
	return Level;
}

int32 FDefaultStatBlock::GetExp() const {
	return Exp;
}

int32 FDefaultStatBlock::GetExpForNextLevel() const {
	if (Level == GetMaxLevel())
		return 0;

	return GrowthRate->ExpForLevel(Level + 1);
}

const FNature& FDefaultStatBlock::GetNature() const {
	if (Nature.IsSet())
		return *Nature.GetValue();

	auto &DataTable = FDataManager::GetInstance().GetDataTable<FNature>();
	auto NatureRows = DataTable.GetTableRowNames();
	auto Index = static_cast<int32>(PersonalityValue % NatureRows.Num());
	return *DataTable.GetData(NatureRows[Index]);
}

IStatEntry& FDefaultStatBlock::GetStat(FName Stat) {
	check(Stats.Contains(Stat))
	return *Stats[Stat];
}

const IStatEntry& FDefaultStatBlock::GetStat(FName Stat) const {
	check(Stats.Contains(Stat))
	return *Stats[Stat];
}

void FDefaultStatBlock::CalculateStats(const TMap<FName, int32>& BaseStats) {
	auto& NatureData = GetNature();

	for (const auto& [StatID, Stat] : Stats) {
		check(BaseStats.Contains(StatID))
		Stat->RefreshValue(Level, BaseStats[StatID], NatureData);
	}
}
