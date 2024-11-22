// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Mocking/UnrealMock.h"
#include "Trainers/Trainer.h"
#include "Trainers/TrainerType.h"
#include <gmock/gmock.h>

/**
 *
 */
class FMockTrainer : public ITrainer {
    DECLARE_MOCK_INTERFACE(FMockTrainer)
    
  public:
    MOCK_METHOD2(Initialize, TScriptInterface<ITrainer>(FName NewTrainerType, FText NewTrainerName));
    MOCK_METHOD(const FGuid &, GetInternalId, (), (const, override));
    MOCK_METHOD(const FTrainerType &, GetTrainerType, (), (const, override));
    MOCK_METHOD(FText, GetTrainerName, (), (const, override));
    MOCK_METHOD(FText, GetFullTrainerName, (), (const, override));
    MOCK_METHOD(int32, GetPayout, (), (const, override));
    MOCK_METHOD(void, HealParty, (), (override));
    MOCK_METHOD(const TArray<TScriptInterface<IPokemon>> &, GetParty, (), (const, override));
    MOCK_METHOD(bool, IsPartyFull, (), (const, override));
    MOCK_METHOD(TScriptInterface<IPokemon>, GetPokemon, (int32 Index), (const, override));
    MOCK_METHOD(int32, GetAblePokemonCount, (), (const, override));
    MOCK_METHOD(void, AddPokemonToParty, (const TScriptInterface<IPokemon> &Pokemon), (override));
    MOCK_METHOD2(SwapPositionsInParty, void(int32 Index1, int32 Index2));
    MOCK_METHOD(void, ClearParty, (), (override));
    MOCK_METHOD(int32, GetIdNumber, (), (const, override));
    MOCK_METHOD(int32, GetSecretId, (), (const, override));
};
