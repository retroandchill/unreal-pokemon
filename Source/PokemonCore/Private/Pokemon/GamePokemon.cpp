// "Unreal Pokémon" created by Retro & Chill.
#include "Pokemon/GamePokemon.h"

#include "DataManager.h"
#include "Managers/PokemonSubsystem.h"
#include "Pokemon/Stats/DefaultStatBlock.h"
#include "Species/GenderRatio.h"
#include "Species/SpeciesData.h"
#include "Utilities/PersonalityValueUtils.h"

// TODO: Instantiate the stat block dynamically based on a user config
FGamePokemon::FGamePokemon(FName Species, int32 Level) : Species(Species),
                                                         PersonalityValue(
	                                                         UPersonalityValueUtils::GeneratePersonalityValue()) {
	const auto& DataManager = FDataManager::GetInstance();
	auto& SpeciesTable = DataManager.GetDataTable<FSpeciesData>();

	auto SpeciesData = SpeciesTable.GetData(Species);
	check(SpeciesData != nullptr)
	StatBlock = MakeUnique<FDefaultStatBlock>(SpeciesData->GrowthRate, Level);
	StatBlock->CalculateStats(SpeciesData->BaseStats);
	CurrentHP = StatBlock->GetStat(UPokemonSubsystem::GetInstance().GetHPStat()).GetStatValue();
}

FText FGamePokemon::GetName() const {
	return Nickname.IsSet() ? Nickname.GetValue() : GetSpecies().RealName;
}

EPokemonGender FGamePokemon::GetGender() const {
	using enum EPokemonGender;

	if (Gender.IsSet())
		return Gender.GetValue();

	auto& GenderRatio = GetSpecies().GetGenderRatio();
	if (GenderRatio.IsGenderless)
		return Genderless;

	return (PersonalityValue & UPersonalityValueUtils::LOWER_8_BITS) < GenderRatio.FemaleChance ? Female : Male;
}

int32 FGamePokemon::GetCurrentHP() const {
	return CurrentHP;
}

int32 FGamePokemon::GetMaxHP() const {
	return GetStatBlock().GetStat(UPokemonSubsystem::GetInstance().GetHPStat()).GetStatValue();
}

bool FGamePokemon::IsFainted() const {
	return CurrentHP <= 0;
}

const FSpeciesData& FGamePokemon::GetSpecies() const {
	auto& DataTable = FDataManager::GetInstance().GetDataTable<FSpeciesData>();
	auto SpeciesData = DataTable.GetData(Species);
	check(SpeciesData != nullptr)

	return *SpeciesData;
}

const IStatBlock& FGamePokemon::GetStatBlock() const {
	return *StatBlock;
}
