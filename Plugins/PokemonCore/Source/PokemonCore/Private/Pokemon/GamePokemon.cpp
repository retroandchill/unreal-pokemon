// "Unreal Pokémon" created by Retro & Chill.
#include "Pokemon/GamePokemon.h"
#include "Bag/Item.h"
#include "DataManager.h"
#include "Managers/PokemonSubsystem.h"
#include "Pokemon/PokemonDTO.h"
#include "Pokemon/Stats/DefaultStatBlock.h"
#include "Settings/PokemonSettings.h"
#include "Species/GenderRatio.h"
#include "Species/SpeciesData.h"
#include "Utilities/ConstructionUtilities.h"
#include "Utilities/PersonalityValueUtils.h"

void UGamePokemon::Initialize(const FPokemonDTO &DTO, const TScriptInterface<ITrainer> &Trainer) {
    Species = DTO.Species;
    PersonalityValue = UPersonalityValueUtils::GeneratePersonalityValue(DTO);
    Nickname = DTO.Nickname;
    Gender = DTO.Gender;
    Shiny = DTO.Shiny;
    StatBlock = UConstructionUtilities::CreateStatBlock(this, DTO);
    StatBlock->CalculateStats(GetSpecies().BaseStats);
    CurrentHP = GetMaxHP();
    MoveBlock = UConstructionUtilities::CreateMoveBlock(this, DTO);
    AbilityBlock = UConstructionUtilities::CreateAbilityBlock(this, DTO);
    HoldItem = DTO.Item;

    if (DTO.PokeBall.IsSet()) {
        PokeBall = *DTO.PokeBall;
    } else {
        PokeBall = GetDefault<UPokemonSettings>()->GetDefaultPokeBall();
    }

    if (Trainer != nullptr) {
        OwnerInfo = FOwnerInfo(*Trainer);
    } else {
        OwnerInfo = FOwnerInfo();
    }

    ObtainedBlock = UConstructionUtilities::CreateObtainedBlock(this, DTO);
}

FText UGamePokemon::GetNickname() const {
    return Nickname.IsSet() ? Nickname.GetValue() : GetSpecies().RealName;
}

EPokemonGender UGamePokemon::GetGender() const {
    using enum EPokemonGender;

    if (Gender.IsSet())
        return Gender.GetValue();

    auto &GenderRatio = GetSpecies().GetGenderRatio();
    if (GenderRatio.IsGenderless)
        return Genderless;

    return (PersonalityValue & UPersonalityValueUtils::LOWER_8_BITS) < GenderRatio.FemaleChance ? Female : Male;
}

const TArray<FName> &UGamePokemon::GetTypes() const {
    return GetSpecies().Types;
}

FName UGamePokemon::GetPokeBall() const {
    return PokeBall;
}

bool UGamePokemon::IsShiny() const {
    if (Shiny.IsSet()) {
        return Shiny.GetValue();
    }

    uint32 A = PersonalityValue ^ static_cast<uint32>(OwnerInfo.ID);
    uint32 B = A & 0xFFFF;
    uint32 C = (A >> 16) & 0xFFFF;
    uint32 D = B ^ C;
    return D < GetDefault<UPokemonSettings>()->GetShinyChance();
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

const FSpeciesData &UGamePokemon::GetSpecies() const {
    const auto &DataManager = FDataManager::GetInstance();
    auto &SpeciesTable = DataManager.GetDataTable<FSpeciesData>();

    auto SpeciesData = SpeciesTable.GetData(Species);
    check(SpeciesData != nullptr)
    return *SpeciesData;
}

uint32 UGamePokemon::GetPersonalityValue() const {
    return PersonalityValue;
}

TScriptInterface<IStatBlock> UGamePokemon::GetStatBlock() const {
    return StatBlock;
}

TScriptInterface<IMoveBlock> UGamePokemon::GetMoveBlock() const {
    return MoveBlock;
}

TScriptInterface<IAbilityBlock> UGamePokemon::GetAbility() const {
    return AbilityBlock;
}

const FItem *UGamePokemon::GetHoldItem() const {
    if (!HoldItem.IsSet()) {
        return nullptr;
    }

    auto ItemData = FDataManager::GetInstance().GetDataTable<FItem>().GetData(HoldItem.GetValue());
    check(ItemData != nullptr)
    return ItemData;
}

const FOwnerInfo &UGamePokemon::GetOwnerInfo() const {
    return OwnerInfo;
}

TScriptInterface<IObtainedBlock> UGamePokemon::GetObtainedInformation() const {
    return ObtainedBlock;
}

UGamePokemon *UGamePokemon::Create(const FPokemonDTO &Data) {
    return Create(Data, nullptr);
}

UGamePokemon *UGamePokemon::Create(const FPokemonDTO &Data, const TScriptInterface<ITrainer> &Trainer) {
    auto Ret = NewObject<UGamePokemon>();
    Ret->Initialize(Data, Trainer);
    return Ret;
}
