// Created by Retro & Chill.


#include "Pokemon/Stats/DefaultStatBlock.h"

#include "DataSubsystem.h"
#include "Species/Stat.h"

/**
 * Randomize the Pokémon's IVs and return a map of the values
 * @return The randomized IVs
 */
inline TMap<FName, int32> RandomizeIVs() {
	auto DataSubsystem = GEngine->GetWorld()->GetGameInstance()->GetSubsystem<UDataSubsystem>();
	check(DataSubsystem != nullptr);
	auto &StatTable = DataSubsystem->GetDataTable<FStat>();

	TMap<FName, int32> Ret;
	StatTable.ForEach([&Ret](const FStat &Stat) {
		if (Stat.Type == EPokemonStatType::Battle)
			return;

		Ret[Stat.ID] = FMath::RandRange(0, 31);
	});

	return Ret;
}

/**
 * Set all of the EVs to 0
 * @return The map of EVs all set to 0
 */
inline TMap<FName, int32> DefaultEVs() {
	auto DataSubsystem = GEngine->GetWorld()->GetGameInstance()->GetSubsystem<UDataSubsystem>();
	check(DataSubsystem != nullptr);
	auto &StatTable = DataSubsystem->GetDataTable<FStat>();

	TMap<FName, int32> Ret;
	StatTable.ForEach([&Ret](const FStat &Stat) {
		if (Stat.Type == EPokemonStatType::Battle)
			return;

		Ret[Stat.ID] = 0;
	});

	return Ret;
}

/**
 * Generate a random nature for the Pokémon
 * @return The randomized Nature for the Pokémon in question
 */
inline FName RandomNature() {
	auto DataSubsystem = GEngine->GetWorld()->GetGameInstance()->GetSubsystem<UDataSubsystem>();
	check(DataSubsystem != nullptr);
	auto &NatureTable = DataSubsystem->GetDataTable<FNature>();

	auto Rows = NatureTable.GetTableRowNames().Array();
	auto Index = FMath::RandRange(0, Rows.Num() - 1);
	return Rows[Index];
}

FDefaultStatCalculator::FDefaultStatCalculator(int32 Level) : Level(Level), IVs(RandomizeIVs()), EVs(DefaultEVs()), Nature(RandomNature()) {
}

int32 FDefaultStatCalculator::CalculateStat(const TMap<FName, int32>& BaseStats, FName Stat) const {
	auto DataSubsystem = GEngine->GetWorld()->GetGameInstance()->GetSubsystem<UDataSubsystem>();
	check(DataSubsystem != nullptr);
	
	auto StatData = DataSubsystem->GetDataTable<FStat>().GetData(Stat);
	check(StatData != nullptr);
	check(StatData->Type != EPokemonStatType::Battle);
	
	check(BaseStats.Contains(Stat));
	if (StatData->Type == EPokemonStatType::Main) {
		return 2 * BaseStats[Stat] + IVs[Stat] + (EVs[Stat] / 4 ) * Level / 100 + Level + 10;
	}
	
	auto NatureData = DataSubsystem->GetDataTable<FNature>().GetData(Nature);
	check(NatureData != nullptr);
		
	auto NatureChange = NatureData->StatChanges.FindByPredicate([&Stat](const FNatureStatChange &Change) {
		Change.Stat == Stat;
	});
	check(NatureChange != nullptr);
	int32 NatureModifer = 100 + NatureChange->Change;
		
	return (2 * BaseStats[Stat] + IVs[Stat] + (EVs[Stat] / 4 ) * Level / 100 + 5) * NatureModifer / 100;
}

int32 FDefaultStatCalculator::GetLevel() const {
	return Level;
}

TMap<FName, int32>& FDefaultStatCalculator::GetIVs() {
	return IVs;
}

const TMap<FName, int32>& FDefaultStatCalculator::GetIVs() const {
	return IVs;
}

TMap<FName, int32>& FDefaultStatCalculator::GetEVs() {
	return EVs;
}

const TMap<FName, int32>& FDefaultStatCalculator::GetEVs() const {
	return EVs;
}

const FNature& FDefaultStatCalculator::GetNature() const {
	auto DataSubsystem = GEngine->GetWorld()->GetGameInstance()->GetSubsystem<UDataSubsystem>();
	check(DataSubsystem != nullptr);
	auto &NatureTable = DataSubsystem->GetDataTable<FNature>();

	auto Ret = NatureTable.GetData(Nature);
	check(Ret != nullptr);
	return *Ret;
}
