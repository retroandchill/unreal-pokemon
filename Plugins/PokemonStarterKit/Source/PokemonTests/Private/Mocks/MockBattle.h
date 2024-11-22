// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Mocking/UnrealMock.h"
#include "Battle/Actions/BattleAction.h"
#include "Battle/Battle.h"
#include "Battle/Transitions/BattleInfo.h"
#include <gmock/gmock.h>

class FMockBattle : public IBattle {
    DECLARE_MOCK_INTERFACE(FMockBattle)
    
  public:
    MOCK_METHOD(TScriptInterface<IBattle>, Initialize, (TArray<TScriptInterface<IBattleSide>> && SidesIn), (override));
    MOCK_METHOD(TScriptInterface<IBattle>, Initialize, (const FBattleInfo &BattleInfo), (override));
    MOCK_METHOD(void, StartBattle, (), (override));
    MOCK_METHOD(void, OnBattlersEnteringBattle,
                (UE::Ranges::TAnyView<TScriptInterface<IBattler>> Battlers), (override));
    MOCK_METHOD(void, QueueAction, (TUniquePtr<IBattleAction> && Action), (override));
    MOCK_METHOD(bool, ActionSelectionFinished, (), (const, override));
    MOCK_METHOD(APawn *, GetBattlePawn, (), (const, override));
    MOCK_METHOD(const TScriptInterface<IBattleSide> &, GetPlayerSide, (), (const, override));
    MOCK_METHOD(const TScriptInterface<IBattleSide> &, GetOpposingSide, (), (const, override));
    MOCK_METHOD(UE::Ranges::TAnyView<TScriptInterface<IBattleSide>>, GetSides, (), (const, override));
    MOCK_METHOD(UE::Ranges::TAnyView<TScriptInterface<IBattler>>, GetActiveBattlers, (), (const, override));
    MOCK_METHOD(void, ExecuteAction, (IBattleAction & Action), (override));
    MOCK_METHOD(void, BindToOnBattleEnd, (FOnBattleEnd::FDelegate && Callback), (override));
};