#include "Asserts.h"
#include "Battle/Battle.h"
#include "Battle/Moves/BaseBattleMove.h"
#include "Misc/AutomationTest.h"
#include "Mocking/UnrealMock.h"
#include "Pokemon/Moves/Move.h"
#include "Battle/Battlers/Battler.h"
#include "Mocks/MockBattle.h"
#include "Mocks/MockBattler.h"
#include "Mocks/MockMove.h"

using namespace testing;

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestMoveHits_Certain, "Unit Tests.Battle.TestMoveHits.CertainHit",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestMoveHits_Certain::RunTest(const FString &Parameters) {
    CREATE_MOCK(IBattle, Battle, FMockBattle, MockBattle);
    CREATE_MOCK(IMove, Move, FMockMove, MockMove);
    ON_CALL(MockMove, GetAccuracy).WillByDefault(Return(0));

    auto BattleMove = NewObject<UBaseBattleMove>();
    BattleMove->Initialize(Battle, Move);
    
    CREATE_MOCK(IBattler, User, FMockBattler, MockUser);
    CREATE_MOCK(IBattler, Target, FMockBattler, MockTarget);
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
    CREATE_MOCK(IBattle, Battle, FMockBattle, MockBattle);
    CREATE_MOCK(IMove, Move, FMockMove, MockMove);
    ON_CALL(MockMove, GetAccuracy).WillByDefault(Return(80));

    auto BattleMove = NewObject<UBaseBattleMove>();
    BattleMove->Initialize(Battle, Move);
    
    CREATE_MOCK(IBattler, User, FMockBattler, MockUser);
    CREATE_MOCK(IBattler, Target, FMockBattler, MockTarget);
    
    UE_CHECK_TRUE(IBattleMove::Execute_PerformHitCheck(BattleMove, User, Target));
    UE_CHECK_FALSE(IBattleMove::Execute_PerformHitCheck(BattleMove, User, Target));
    UE_CHECK_TRUE(IBattleMove::Execute_PerformHitCheck(BattleMove, User, Target));
    UE_CHECK_TRUE(IBattleMove::Execute_PerformHitCheck(BattleMove, User, Target));
    UE_CHECK_FALSE(IBattleMove::Execute_PerformHitCheck(BattleMove, User, Target));
    
    return true;
}