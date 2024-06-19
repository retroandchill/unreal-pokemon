#include "Asserts.h"
#include "Battle/Battle.h"
#include "Battle/Moves/BaseBattleMove.h"
#include "Misc/AutomationTest.h"
#include "Mocking/UnrealMock.h"
#include "Pokemon/Moves/Move.h"
#include "Battle/Battlers/Battler.h"

using namespace fakeit;

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestMoveHits_Certain, "Unit Tests.Battle.TestMoveHits.CertainHit",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestMoveHits_Certain::RunTest(const FString &Parameters) {
    auto [Battle, MockBattle] = UnrealMock::CreateMock<IBattle>();
    auto [Move, MockMove] = UnrealMock::CreateMock<IMove>();
    ON_CALL(MockMove, GetAccuracy).WillByDefault(Return(0));

    auto BattleMove = NewObject<UBaseBattleMove>();
    BattleMove->Initialize(Battle, Move);
    
    auto [User, MockUser] = UnrealMock::CreateMock<IBattler>();
    auto [Target, MockTarget] = UnrealMock::CreateMock<IBattler>();
    for (int i = 0; i < 100; i++) {
        UE_CHECK_TRUE(IBattleMove::Execute_PerformHitCheck(BattleMove, User, Target));
    }
    
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestMoveHits_Regular, "Unit Tests.Battle.TestMoveHits.RegularMove",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestMoveHits_Regular::RunTest(const FString &Parameters) {
    // Make the test pass by returning true, or fail by returning false.
    FMath::RandInit(6412);
    auto [Battle, MockBattle] = UnrealMock::CreateMock<IBattle>();
    auto [Move, MockMove] = UnrealMock::CreateMock<IMove>();
    ON_CALL(MockMove, GetAccuracy).WillByDefault(Return(80));

    auto BattleMove = NewObject<UBaseBattleMove>();
    BattleMove->Initialize(Battle, Move);
    
    auto [User, MockUser] = UnrealMock::CreateMock<IBattler>();
    auto [Target, MockTarget] = UnrealMock::CreateMock<IBattler>();
    
    UE_CHECK_TRUE(IBattleMove::Execute_PerformHitCheck(BattleMove, User, Target));
    UE_CHECK_FALSE(IBattleMove::Execute_PerformHitCheck(BattleMove, User, Target));
    UE_CHECK_TRUE(IBattleMove::Execute_PerformHitCheck(BattleMove, User, Target));
    UE_CHECK_TRUE(IBattleMove::Execute_PerformHitCheck(BattleMove, User, Target));
    UE_CHECK_FALSE(IBattleMove::Execute_PerformHitCheck(BattleMove, User, Target));
    
    return true;
}