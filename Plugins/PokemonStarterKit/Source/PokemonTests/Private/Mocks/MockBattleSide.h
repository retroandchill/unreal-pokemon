﻿// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Battle/BattleSide.h"
#include "Mocking/UnrealMock.h"

THIRD_PARTY_INCLUDES_START
#include <gmock/gmock.h>
THIRD_PARTY_INCLUDES_END

/**
 *
 */
class FMockBattleSide : public IBattleSide {
    DECLARE_MOCK_INTERFACE(FMockBattleSide)

  public:
    MOCK_METHOD4(Initialize,
                 UE5Coro::TCoroutine<TScriptInterface<IBattleSide>>(TScriptInterface<IBattle> Battle,
                                                                    TArray<TScriptInterface<IPokemon>> Pokemon,
                                                                    bool ShowBackSprites, FForceLatentCoroutine));
    MOCK_METHOD5(Initialize, UE5Coro::TCoroutine<TScriptInterface<IBattleSide>>(
                                 TScriptInterface<IBattle> Battle, TScriptInterface<ITrainer> Trainer,
                                 uint8 PokemonCount, bool ShowBackSprites, FForceLatentCoroutine));
    MOCK_METHOD(const FGuid &, GetInternalId, (), (const, override));
    MOCK_METHOD(const TScriptInterface<IBattle> &, GetOwningBattle, (), (const, override));
    MOCK_METHOD(UTurnBasedEffectComponent *, GetTurnBasedEffectComponent, (), (const, override));
    MOCK_METHOD(Retro::TGenerator<UTurnBasedEffectComponent *>, GetChildEffectComponents, (), (const, override));
    MOCK_METHOD(uint8, GetSideSize, (), (const, override));
    MOCK_METHOD(const FText &, GetIntroText, (), (const, override));
    MOCK_METHOD(const TOptional<FText> &, GetSendOutText, (), (const, override));
    MOCK_METHOD(bool, ShowBackSprites, (), (const, override));
    MOCK_METHOD(void, SendOutBattlers, (), (const, override));
    MOCK_METHOD(const TArray<TScriptInterface<IBattler>> &, GetBattlers, (), (const, override));
    MOCK_METHOD(const TArray<TScriptInterface<ITrainer>> &, GetTrainers, (), (const, override));
    MOCK_METHOD(const TArray<TScriptInterface<IBattler>> &, GetTrainerParty,
                (const TScriptInterface<ITrainer> &Trainer), (const, override));
    MOCK_METHOD3(SwapBattlerPositions, void(const TScriptInterface<ITrainer> &Trainer, int32 IndexA, int32 IndexB));
    MOCK_METHOD(bool, CanBattle, (), (const, override));
};
