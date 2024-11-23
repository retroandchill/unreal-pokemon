// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Mocking/UnrealMock.h"
#include "Pokemon/Pokemon.h"
#include "Pokemon/PokemonDTO.h"
#include "Species/SpeciesData.h"
#include "Trainers/OwnerInfo.h"
#include <gmock/gmock.h>

/**
 *
 */
class FMockPokemon : public IPokemon {
    DECLARE_MOCK_INTERFACE(FMockPokemon)

  public:
    MOCK_METHOD2(Initialize, void(const FPokemonDTO &DTO, const TScriptInterface<ITrainer> &Trainer));
    MOCK_METHOD(FText, GetNickname, (), (const, override));
    MOCK_METHOD(uint32, GetPersonalityValue, (), (const, override));
    MOCK_METHOD(const FSpeciesData &, GetSpecies, (), (const, override));
    MOCK_METHOD(EPokemonGender, GetGender, (), (const, override));
    MOCK_METHOD(const TArray<FName> &, GetTypes, (), (const, override));
    MOCK_METHOD(FName, GetPokeBall, (), (const, override));
    MOCK_METHOD(bool, IsShiny, (), (const, override));
    MOCK_METHOD(bool, IsAble, (), (const, override));
    MOCK_METHOD(int32, GetCurrentHP, (), (const, override));
    MOCK_METHOD(void, SetCurrentHP, (int32 Value), (override));
    MOCK_METHOD(int32, GetMaxHP, (), (const, override));
    MOCK_METHOD(bool, IsFainted, (), (const, override));
    MOCK_METHOD(TScriptInterface<IStatBlock>, GetStatBlock, (), (const, override));
    MOCK_METHOD(TScriptInterface<IMoveBlock>, GetMoveBlock, (), (const, override));
    MOCK_METHOD(TScriptInterface<IAbilityBlock>, GetAbility, (), (const, override));
    MOCK_METHOD(const FItem *, GetHoldItem, (), (const, override));
    MOCK_METHOD(void, SetHoldItem, (FName Item), (override));
    MOCK_METHOD(void, RemoveHoldItem, (), (override));
    MOCK_METHOD(const FOwnerInfo &, GetOwnerInfo, (), (const, override));
    MOCK_METHOD(TScriptInterface<IObtainedBlock>, GetObtainedInformation, (), (const, override));
    MOCK_METHOD(const TScriptInterface<ITrainer> &, GetCurrentHandler, (), (const, override));
    MOCK_METHOD(void, SetCurrentHandler, (const TScriptInterface<ITrainer> &NewHandler), ());
};
