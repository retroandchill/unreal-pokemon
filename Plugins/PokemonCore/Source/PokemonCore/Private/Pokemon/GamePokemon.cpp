// "Unreal Pokémon" created by Retro & Chill.
#include "Pokemon/GamePokemon.h"

#include "DataManager.h"
#include "Managers/PokemonSubsystem.h"
#include "Pokemon/PokemonDTO.h"
#include "Pokemon/Stats/DefaultStatBlock.h"
#include "Species/GenderRatio.h"
#include "Species/SpeciesData.h"
#include "Utilities/ConstructionUtilities.h"
#include "Utilities/PersonalityValueUtils.h"

void UGamePokemon::Initialize(const FPokemonDTO &DTO) {
    Species = DTO.Species;
    PersonalityValue = UPersonalityValueUtils::GeneratePersonalityValue(DTO);
    Gender = DTO.Gender;
    Shiny = DTO.Shiny;
    StatBlock = UConstructionUtilities::CreateStatBlock(this, DTO);
    StatBlock->CalculateStats(GetSpecies().BaseStats);
    CurrentHP = GetMaxHP();
}

FText UGamePokemon::GetNickname() const { return Nickname.IsSet() ? Nickname.GetValue() : GetSpecies().RealName; }

EPokemonGender UGamePokemon::GetGender() const {
    using enum EPokemonGender;

    if (Gender.IsSet())
        return Gender.GetValue();

    auto &GenderRatio = GetSpecies().GetGenderRatio();
    if (GenderRatio.IsGenderless)
        return Genderless;

    return (PersonalityValue & UPersonalityValueUtils::LOWER_8_BITS) < GenderRatio.FemaleChance ? Female : Male;
}

int32 UGamePokemon::GetCurrentHP() const { return CurrentHP; }

int32 UGamePokemon::GetMaxHP() const {
    return GetStatBlock()->GetStat(UPokemonSubsystem::GetInstance().GetHPStat())->GetStatValue();
}

bool UGamePokemon::IsFainted() const { return CurrentHP <= 0; }

const FSpeciesData &UGamePokemon::GetSpecies() const {
    const auto &DataManager = FDataManager::GetInstance();
    auto &SpeciesTable = DataManager.GetDataTable<FSpeciesData>();

    auto SpeciesData = SpeciesTable.GetData(Species);
    check(SpeciesData != nullptr)
return *SpeciesData;
}

uint32 UGamePokemon::GetPersonalityValue() const { return PersonalityValue; }

TScriptInterface<IStatBlock> UGamePokemon::GetStatBlock() const { return StatBlock; }

UGamePokemon *UGamePokemon::Create(const FPokemonDTO &Data) {
    auto Ret = NewObject<UGamePokemon>();
    Ret->Initialize(Data);
    return Ret;
}
