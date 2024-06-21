// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Battle/Moves/BattleMove.h"
#include <gmock/gmock.h>

/**
 * 
 */
class FMockBattleMove : public IBattleMove {
public:
    ~FMockBattleMove() override = default;

    MOCK_METHOD2(Initialize, TScriptInterface<IBattleMove>(const TScriptInterface<IBattle> &Battle, const TScriptInterface<IMove> &Move));
    MOCK_METHOD(bool, IsConfusionAttack, (), (const, override));
    MOCK_METHOD(bool, HasHighCriticalHitRate, (), (const, override));
    MOCK_METHOD(bool, HasTag, (FName Tag), (const, override));
};
