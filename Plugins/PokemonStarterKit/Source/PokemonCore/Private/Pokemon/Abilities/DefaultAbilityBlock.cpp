﻿// "Unreal Pokémon" created by Retro & Chill.

#include "Pokemon/Abilities/DefaultAbilityBlock.h"
#include "DataManager.h"
#include "Pokemon/Pokemon.h"
#include "Pokemon/PokemonDTO.h"
#include "PokemonDataSettings.h"
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
    return AbilityData != nullptr ? AbilityData->RealName : GetDefault<UPokemonDataSettings>()->NoAbilityName;
}

const FText &UDefaultAbilityBlock::GetAbilityDescription() const {
    auto AbilityID = GetAbilityID();
    auto AbilityData = FDataManager::GetInstance().GetDataTable<FAbility>().GetData(AbilityID);
    return AbilityData != nullptr ? AbilityData->Description : GetDefault<UPokemonDataSettings>()->NoAbilityDescription;
}

int32 UDefaultAbilityBlock::GetAbilityIndex() const {
    check(Owner != nullptr)
    int32 NormalAbilityCount = GetDefault<UPokemonDataSettings>()->MaxDefaultAbilities;
    return static_cast<int32>(Owner->GetPersonalityValue() % NormalAbilityCount);
}