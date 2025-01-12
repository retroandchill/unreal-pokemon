﻿// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Battle/Actions/BattleAction.h"
#include "Battle/Battle.h"
#include "Battle/Transitions/BattleInfo.h"
#include "Mocking/UnrealMock.h"
#include <gmock/gmock.h>

class FMockBattle : public IBattle {
    DECLARE_MOCK_INTERFACE(FMockBattle)

  public:
    MOCK_METHOD(TScriptInterface<IBattle>, Initialize, (TArray<TScriptInterface<IBattleSide>> && SidesIn), (override));
    MOCK_METHOD(TScriptInterface<IBattle>, Initialize, (const FBattleInfo &BattleInfo), (override));
    MOCK_METHOD2(ConductBattle, UE5Coro::TCoroutine<EBattleResult>(APlayerController* Controller, FForceLatentCoroutine Coro));
    MOCK_METHOD(UE5Coro::TCoroutine<>, StartBattle, (), (override));
    MOCK_METHOD(UE5Coro::TCoroutine<>, OnBattlersEnteringBattle, (Retro::Ranges::TAnyView<TScriptInterface<IBattler>> Battlers),
                (override));
    MOCK_METHOD(void, QueueAction, (TUniquePtr<IBattleAction> && Action), (override));
    MOCK_METHOD(bool, ActionSelectionFinished, (), (const, override));
    MOCK_METHOD(APawn *, GetBattlePawn, (), (const, override));
    MOCK_METHOD(const TScriptInterface<IBattleSide> &, GetPlayerSide, (), (const, override));
    MOCK_METHOD(const TScriptInterface<IBattleSide> &, GetOpposingSide, (), (const, override));
    MOCK_METHOD(Retro::TGenerator<TScriptInterface<IBattleSide>>, GetSides, (), (const, override));
    MOCK_METHOD(Retro::TGenerator<TScriptInterface<IBattler>>, GetActiveBattlers, (), (const, override));
    MOCK_METHOD2(ExecuteAction, UE5Coro::TCoroutine<>(IBattleAction & Action, FForceLatentCoroutine Coro));
};