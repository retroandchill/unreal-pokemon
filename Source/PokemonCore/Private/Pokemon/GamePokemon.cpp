// "Unreal Pokémon" created by Retro & Chill.
#include "Pokemon/GamePokemon.h"

#include "DataManager.h"
#include "Managers/PokemonSubsystem.h"
#include "Pokemon/Stats/DefaultStatBlock.h"
#include "Species/GenderRatio.h"
#include "Species/SpeciesData.h"
#include "Utilities/PersonalityValueUtils.h"

/**
 * Helper function to locate the species that this Pokémon refers to
 * @param Species The ID of the species to find
 * @return The located species data
 */
TRowPointer<FSpeciesData> FindSpeciesData(FName Species) {
	const auto& DataManager = FDataManager::GetInstance();
	auto& SpeciesTable = DataManager.GetDataTable<FSpeciesData>();

	auto SpeciesData = SpeciesTable.GetDataManaged(Species);
	check(SpeciesData != nullptr)
	return SpeciesData;
}

// TODO: Instantiate the stat block dynamically based on a user config
FGamePokemon::FGamePokemon(FName SpeciesID, int32 Level) : Species(FindSpeciesData(SpeciesID)), StatBlock(MakeUnique<FDefaultStatBlock>(Species->GrowthRate, Level)) {
	CommonInit();
}

FGamePokemon::FGamePokemon(FName SpeciesID, int32 Level, EPokemonGender Gender, const TMap<FName, int32>& IVs,
	const TMap<FName, int32>& EVs, FName Nature, int32 Ability, TArray<TSharedRef<IMove>>&& Moves, bool Shiny, FName Item) : Species(FindSpeciesData(SpeciesID)), Gender(Gender), Shiny(Shiny), StatBlock(MakeUnique<FDefaultStatBlock>(Species->GrowthRate, Level, IVs, EVs, Nature)), Moves(MoveTemp(Moves)) {
}

FText FGamePokemon::GetName() const {
	return Nickname.IsSet() ? Nickname.GetValue() : GetSpecies().RealName;
}

EPokemonGender FGamePokemon::GetGender() const {
	using enum EPokemonGender;

	if (Gender.IsSet())
		return Gender.GetValue();

	auto& GenderRatio = Species->GetGenderRatio();
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
	return *Species;
}

const IStatBlock& FGamePokemon::GetStatBlock() const {
	return *StatBlock;
}

void FGamePokemon::CommonInit() {
	StatBlock->CalculateStats(Species->BaseStats);
	CurrentHP = StatBlock->GetStat(UPokemonSubsystem::GetInstance().GetHPStat()).GetStatValue();
}
