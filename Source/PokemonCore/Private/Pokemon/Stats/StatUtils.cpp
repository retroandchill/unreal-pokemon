// "Unreal Pokémon" created by Retro & Chill.
#include "Pokemon/Stats/StatUtils.h"

#include "DataManager.h"
#include "Species/Nature.h"
#include "Species/Stat.h"

using namespace StatUtils;

POKEMONCORE_API TMap<FName, int32> StatUtils::RandomizeIVs() {
	auto &DataSubsystem = FDataManager::GetInstance();
	auto &StatTable = DataSubsystem.GetDataTable<FStat>();

	TMap<FName, int32> Ret;
	StatTable.ForEach([&Ret](const FStat &Stat) {
		if (Stat.Type == EPokemonStatType::Battle)
			return;

		Ret[Stat.ID] = FMath::RandRange(0, 31);
	});

	return Ret;
}

POKEMONCORE_API TMap<FName, int32> StatUtils::DefaultEVs() {
	auto &DataSubsystem = FDataManager::GetInstance();
	auto &StatTable = DataSubsystem.GetDataTable<FStat>();

	TMap<FName, int32> Ret;
	StatTable.ForEach([&Ret](const FStat &Stat) {
		if (Stat.Type == EPokemonStatType::Battle)
			return;

		Ret[Stat.ID] = 0;
	});

	return Ret;
}

POKEMONCORE_API FName StatUtils::RandomNature() {
	auto &DataSubsystem = FDataManager::GetInstance();
	auto &NatureTable = DataSubsystem.GetDataTable<FNature>();

	auto Rows = NatureTable.GetTableRowNames().Array();
	auto Index = FMath::RandRange(0, Rows.Num() - 1);
	return Rows[Index];
}

POKEMONCORE_API TUniquePtr<Exp::IGrowthRate> StatUtils::CreateGrowthRate(FName GrowthRate) {
	return Exp::FGrowthRateRegistry::GetInstance().Construct(GrowthRate);
}

POKEMONCORE_API int32 StatUtils::GetMaxLevel() {
	// TODO: Allow this to be configurable
	return 100;
}
