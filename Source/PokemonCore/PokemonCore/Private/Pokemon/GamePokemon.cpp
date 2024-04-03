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

void UGamePokemon::Initialize(const FPokemonDTO& DTO) {
	Species = DTO.Species;
	PersonalityValue = UPersonalityValueUtils::GeneratePersonalityValue(DTO);
	Gender = DTO.Gender;
	Shiny = DTO.bShiny;
	StatBlock = CreateStatBlock(this, DTO.StatBlock);
	StatBlock->CalculateStats(GetSpecies().BaseStats);
	CurrentHP = GetMaxHP();
}

FText UGamePokemon::GetNickname() const {
	return Nickname.IsSet() ? Nickname.GetValue() : GetSpecies().RealName;
}

EPokemonGender UGamePokemon::GetGender() const {
	using enum EPokemonGender;

	if (Gender.IsSet())
		return Gender.GetValue();

	auto& GenderRatio = GetSpecies().GetGenderRatio();
	if (GenderRatio.IsGenderless)
		return Genderless;

	return (PersonalityValue & UPersonalityValueUtils::LOWER_8_BITS) < GenderRatio.FemaleChance ? Female : Male;
}

int32 UGamePokemon::GetCurrentHP() const {
	return CurrentHP;
}

int32 UGamePokemon::GetMaxHP() const {
	return GetStatBlock()->GetStat(UPokemonSubsystem::GetInstance().GetHPStat())->GetStatValue();
}

bool UGamePokemon::IsFainted() const {
	return CurrentHP <= 0;
}

const FSpeciesData& UGamePokemon::GetSpecies() const {
	const auto& DataManager = FDataManager::GetInstance();
	auto& SpeciesTable = DataManager.GetDataTable<FSpeciesData>();

	auto SpeciesData = SpeciesTable.GetData(Species);
	ASSERT(SpeciesData != nullptr)
	return *SpeciesData;
}

uint32 UGamePokemon::GetPersonalityValue() const {
	return PersonalityValue;
}

TScriptInterface<IStatBlock> UGamePokemon::GetStatBlock() const {
	return StatBlock;
}

UPokemonBuilder* UGamePokemon::ToBuilder() const {
	auto Builder = NewObject<UPokemonBuilder>()
		->Species(Species)
		->PersonalityValue(PersonalityValue)
		->CurrentHP(CurrentHP)
		->StatBlock(StatBlock->ToDTO());

	ADD_OPTIONAL(*Builder, Gender);
	ADD_OPTIONAL(*Builder, Shiny);
	ADD_OPTIONAL(*Builder, Nickname);

	return Builder;
	
}

bool UGamePokemon::Equals(const TScriptInterface<IPokemon>& Other) const {
	if (Other.GetObject()->GetClass() == StaticClass()) {
		return *this == static_cast<UGamePokemon&>(*Other);
	}

	return false;
}

UGamePokemon* UGamePokemon::Create(const FPokemonDTO& Data) {
	auto Ret = NewObject<UGamePokemon>();
	Ret->Initialize(Data);
	return Ret;
}

bool UGamePokemon::operator==(const UGamePokemon& Other) const {
	if (Species != Other.Species || !OptionalsSame(Gender, Other.Gender) || !OptionalsSame(Nickname, Other.Nickname) ||
			!OptionalsSame(Shiny, Other.Shiny) || StatBlock->Equals(Other.StatBlock) || Moves.Num() != Other.Moves.Num())
		return false;
		
	for (int i = 0; i < Moves.Num(); i++) {
		if (Moves[i]->Equals(Other.Moves[i]))
			return false;
	}

	return true;
}
