// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Battle/BattleSide.h"
#include "Mocking/UnrealMock.h"
#include <gmock/gmock.h>

/**
 *
 */
class FMockBattleSide : public IBattleSide {
    DECLARE_MOCK_INTERFACE(FMockBattleSide)

  public:
    MOCK_METHOD3(Initialize, TScriptInterface<IBattleSide>(const TScriptInterface<IBattle> &Battle,
                                                           const TArray<TScriptInterface<IPokemon>> &Pokemon,
                                                           bool ShowBackSprites));
    MOCK_METHOD4(Initialize, TScriptInterface<IBattleSide>(const TScriptInterface<IBattle> &Battle,
                                                           const TScriptInterface<ITrainer> &Trainer,
                                                           uint8 PokemonCount, bool ShowBackSprites));
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
