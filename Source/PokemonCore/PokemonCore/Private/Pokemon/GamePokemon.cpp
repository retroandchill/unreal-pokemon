// "Unreal Pokémon" created by Retro & Chill.
#include "Pokemon/GamePokemon.h"

#include "Asserts.h"
#include "DataManager.h"
#include "PokemonCoreSettings.h"
#include "DataTypes/OptionalUtilities.h"
#include "Managers/PokemonSubsystem.h"
#include "Pokemon/PokemonBuilder.h"
#include "Pokemon/PokemonDTO.h"
#include "Pokemon/Stats/DefaultStatBlock.h"
#include "Species/GenderRatio.h"
#include "Species/SpeciesData.h"
#include "Utilities/PersonalityValueUtils.h"
#include "Moves/Move.h"

/**
 * Helper function to locate the species that this Pokémon refers to
 * @param Species The ID of the species to find
 * @return The located species data
 */
TRowPointer<FSpeciesData> FindSpeciesData(FName Species) {
	const auto& DataManager = FDataManager::GetInstance();
	auto& SpeciesTable = DataManager.GetDataTable<FSpeciesData>();

	auto SpeciesData = SpeciesTable.GetDataManaged(Species);
	ASSERT(SpeciesData != nullptr)
	return SpeciesData;
}

UGamePokemon* UGamePokemon::Initialize(const FPokemonDTO& DTO) {
	Species = FindSpeciesData(DTO.Species);
	PersonalityValue = UPersonalityValueUtils::GeneratePersonalityValue(DTO);
	Gender = OPTIONAL(DTO, Gender);
	Shiny = BOOL_OPTIONAL(DTO, Shiny);
	StatBlock = CreateStatBlock(Species->GrowthRate, PersonalityValue, DTO.StatBlock);
	return this;
}

FText UGamePokemon::GetNickname() const {
	return Nickname.IsSet() ? Nickname.GetValue() : GetSpecies().RealName;
}

EPokemonGender UGamePokemon::GetGender() const {
	using enum EPokemonGender;

	if (Gender.IsSet())
		return Gender.GetValue();

	auto& GenderRatio = Species->GetGenderRatio();
	if (GenderRatio.IsGenderless)
		return Genderless;

	return (PersonalityValue & UPersonalityValueUtils::LOWER_8_BITS) < GenderRatio.FemaleChance ? Female : Male;
}

int32 UGamePokemon::GetCurrentHP() const {
	return CurrentHP;
}

int32 UGamePokemon::GetMaxHP() const {
	return GetStatBlock().GetStat(UPokemonSubsystem::GetInstance().GetHPStat()).GetStatValue();
}

bool UGamePokemon::IsFainted() const {
	return CurrentHP <= 0;
}

const FSpeciesData& UGamePokemon::GetSpecies() const {
	return *Species;
}

const IStatBlock& UGamePokemon::GetStatBlock() const {
	return *StatBlock;
}

UPokemonBuilder* UGamePokemon::ToBuilder() const {
	auto Builder = NewObject<UPokemonBuilder>()
		->Species(Species->ID)
		->PersonalityValue(PersonalityValue)
		->CurrentHP(CurrentHP)
		->StatBlock(StatBlock->ToDTO());

	ADD_OPTIONAL(*Builder, Gender);
	ADD_OPTIONAL(*Builder, Shiny);
	ADD_OPTIONAL(*Builder, Nickname);

	return Builder;
	
}

bool UGamePokemon::operator==(const IPokemon& Other) const {
/*
	if (GetClass() == Other) {
		return *this == static_cast<const UGamePokemon&>(Other);
	}
	*/
	
	return false;
}

bool UGamePokemon::operator==(const UGamePokemon& Other) const {
	if (Species != Other.Species || !OptionalsSame(Gender, Other.Gender) || !OptionalsSame(Nickname, Other.Nickname) ||
			!OptionalsSame(Shiny, Other.Shiny) || *StatBlock != *Other.StatBlock || Moves.Num() != Other.Moves.Num())
		return false;
		
	for (int i = 0; i < Moves.Num(); i++) {
		if (*Moves[i] != *Other.Moves[i])
			return false;
	}

	return true;
}
