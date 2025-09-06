// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Battle/Moves/BattleMove.h"
#include "Mocking/UnrealMock.h"
#include "Moves/Target.h"

THIRD_PARTY_INCLUDES_START
#include <gmock/gmock.h>
THIRD_PARTY_INCLUDES_END

/**
 *
 */
class FMockBattleMove : public IBattleMove
{
    DECLARE_MOCK_INTERFACE(FMockBattleMove)

  public:
    MOCK_METHOD2(Initialize, TScriptInterface<IBattleMove>(const TScriptInterface<IBattler> &Battler,
                                                           const TScriptInterface<IMove> &Move));
    MOCK_METHOD(int32, GetBasePower, (), (const, override));
    MOCK_METHOD(int32, GetAccuracy, (), (const, override));
    MOCK_METHOD(const FMoveTarget &, GetTargetType, (), (const, override));
    MOCK_METHOD(const TArray<FName> &, GetTags, (), (const, override));
    MOCK_METHOD(Retro::TGenerator<TScriptInterface<IBattler>>, GetAllPossibleTargets, (), (const, override));
    MOCK_METHOD(bool, IsUsable, (), (const, override));
    MOCK_METHOD(FText, GetDisplayName, (), (const override));
    MOCK_METHOD(int32, GetCurrentPP, (), (const override));
    MOCK_METHOD(int32, GetMaxPP, (), (const override));
    MOCK_METHOD(FName, GetDisplayType, (), (const override));
    MOCK_METHOD(EMoveDamageCategory, GetCategory, (), (const override));
    MOCK_METHOD(int32, GetPriority, (), (const override));
    MOCK_METHOD(int32, GetAdditionalEffectChance, (), (const override));
    MOCK_METHOD(void, PayCost, (int32 Amount), (override));
    MOCK_METHOD(const TScriptInterface<IBattler> &, GetOwningBattler, (), (const, override));
    MOCK_METHOD(UE5Coro::TCoroutine<>, TryActivateMove, (const TArray<FTargetWithIndex> &Targets), (override));
};
