// "Unreal Pokémon" created by Retro & Chill.

#include "Pokemon/Abilities/DefaultAbilityBlock.h"
#include "DataManager.h"
#include "Pokemon/Pokemon.h"
#include "Pokemon/PokemonDTO.h"
#include "Settings/BaseSettings.h"
#include "Species/Ability.h"
#include "Species/SpeciesData.h"

void UDefaultAbilityBlock::Initialize(const TScriptInterface<IPokemon> &NewOwner, const FPokemonDTO &DTO) {
    Owner = NewOwner;

    if (DTO.Ability.IsSet()) {
        Ability = DTO.Ability;
    }
}

FName UDefaultAbilityBlock::GetAbilityID() const {
    if (Ability.IsSet()) {
        return Ability.GetValue();
    }

    check(Owner != nullptr)
    auto &SpeciesData = Owner->GetSpecies();
    int32 SearchAbilityIndex = GetAbilityIndex();

    if (SpeciesData.Abilities.IsEmpty()) {
        return NAME_None;
    }

    return SpeciesData.Abilities[FMath::Clamp(SearchAbilityIndex, 0, SpeciesData.Abilities.Num() - 1)];
}

const FText &UDefaultAbilityBlock::GetDisplayName() const {
    auto AbilityID = GetAbilityID();
    auto AbilityData = FDataManager::GetInstance().GetDataTable<FAbility>().GetData(AbilityID);
    return AbilityData != nullptr ? AbilityData->RealName : Pokemon::FBaseSettings::Get().GetNoAbilityName();
}

const FText &UDefaultAbilityBlock::GetAbilityDescription() const {
    auto AbilityID = GetAbilityID();
    auto AbilityData = FDataManager::GetInstance().GetDataTable<FAbility>().GetData(AbilityID);
    return AbilityData != nullptr ? AbilityData->Description : Pokemon::FBaseSettings::Get().GetNoAbilityDescription();
}

int32 UDefaultAbilityBlock::GetAbilityIndex() const {
    check(Owner != nullptr)
    int32 NormalAbilityCount = Pokemon::FBaseSettings::Get().GetMaxDefaultAbilities();
    return static_cast<int32>(Owner->GetPersonalityValue() % NormalAbilityCount);
}