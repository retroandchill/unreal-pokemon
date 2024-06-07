// "Unreal Pokémon" created by Retro & Chill.
#include "Pokemon/GamePokemon.h"
#include "Bag/Item.h"
#include "DataManager.h"
#include "Lookup/InjectionUtilities.h"
#include "Managers/PokemonSubsystem.h"
#include "Pokemon/Abilities/AbilityBlock.h"
#include "Pokemon/Moves/MoveBlock.h"
#include "Pokemon/PokemonDTO.h"
#include "Pokemon/Stats/DefaultStatBlock.h"
#include "Pokemon/TrainerMemo/ObtainedBlock.h"
#include "Settings/BaseSettings.h"
#include "Species/GenderRatio.h"
#include "Species/SpeciesData.h"
#include "Utilities/PersonalityValueUtils.h"

void UGamePokemon::Initialize(const FPokemonDTO &DTO, const TScriptInterface<ITrainer> &Trainer) {
    Species = DTO.Species;
    PersonalityValue = UPersonalityValueUtils::GeneratePersonalityValue(DTO);
    Nickname = DTO.Nickname;
    Gender = DTO.Gender;
    Shiny = DTO.Shiny;
    StatBlock = UnrealInjector::NewInjectedDependency<IStatBlock>(this, this, DTO);
    StatBlock->CalculateStats(GetSpecies().BaseStats);
    CurrentHP = GetMaxHP();
    MoveBlock = UnrealInjector::NewInjectedDependency<IMoveBlock>(this, DTO);
    AbilityBlock = UnrealInjector::NewInjectedDependency<IAbilityBlock>(this, this, DTO);
    HoldItem = DTO.Item;

    if (DTO.PokeBall.IsSet()) {
        PokeBall = *DTO.PokeBall;
    } else {
        PokeBall = Pokemon::FBaseSettings::Get().GetDefaultPokeBall();
    }

    if (Trainer != nullptr) {
        OwnerInfo = FOwnerInfo(*Trainer);
    } else {
        OwnerInfo = FOwnerInfo(this);
    }

    ObtainedBlock = UnrealInjector::NewInjectedDependency<IObtainedBlock>(this, DTO);
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
    return D < Pokemon::FBaseSettings::Get().GetShinyPokemonChance();
}

int32 UGamePokemon::GetCurrentHP() const {
    return CurrentHP;
}

int32 UGamePokemon::GetMaxHP() const {
    return GetStatBlock()->GetStat(Pokemon::FBaseSettings::Get().GetHPStat())->GetStatValue();
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

void UGamePokemon::SetHoldItem(FName Item) {
    HoldItem.Emplace(Item);
}

void UGamePokemon::RemoveHoldItem() {
    HoldItem.Reset();
}

const FOwnerInfo &UGamePokemon::GetOwnerInfo() const {
    return OwnerInfo;
}

TScriptInterface<IObtainedBlock> UGamePokemon::GetObtainedInformation() const {
    return ObtainedBlock;
}

UGamePokemon *UGamePokemon::Create(UObject *WorldContext, const FPokemonDTO &Data) {
    return Create(WorldContext, Data, nullptr);
}

UGamePokemon *UGamePokemon::Create(UObject *WorldContext, const FPokemonDTO &Data,
                                   const TScriptInterface<ITrainer> &Trainer) {
    auto Ret = NewObject<UGamePokemon>(WorldContext);
    Ret->Initialize(Data, Trainer);
    return Ret;
}
