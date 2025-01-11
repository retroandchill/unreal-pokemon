// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Battle/Actions/BattleAction.h"
#include <gmock/gmock.h>

/**
 *
 */
class FMockBattleAction : public IBattleAction {
  public:
    ~FMockBattleAction() override = default;

    MOCK_METHOD(const TScriptInterface<IBattler> &, GetBattler, (), (const, override));
    MOCK_METHOD(bool, CanExecute, (), (const, override));
    MOCK_METHOD(UE5Coro::TCoroutine<>, Execute, (), (override));
    MOCK_METHOD(bool, IsExecuting, (), (const, override));
    MOCK_METHOD(int32, GetPriority, (), (const, override));
    MOCK_METHOD(FText, GetActionMessage, (), (const, override));
};
