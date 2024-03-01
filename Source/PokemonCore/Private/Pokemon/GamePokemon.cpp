// "Unreal Pokémon" created by Retro & Chill.
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
