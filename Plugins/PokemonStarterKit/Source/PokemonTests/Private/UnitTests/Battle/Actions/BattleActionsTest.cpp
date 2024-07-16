#include "Asserts.h"
#include "Battle/Actions/BattleActionUseMove.h"
#include "Battle/Battlers/Battler.h"
#include "Battle/Battlers/BattlerAbilityComponent.h"
#include "Misc/AutomationTest.h"
#include "Mocking/UnrealMock.h"
#include "Mocks/MockBattleMove.h"
#include "Mocks/MockBattler.h"

using namespace testing;

IMPLEMENT_SIMPLE_AUTOMATION_TEST(BattleActionsTest_Moves, "Unit Tests.Battle.Actions.BattleActionsTest.Moves",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool BattleActionsTest_Moves::RunTest(const FString &Parameters) {
    /*
    auto FakeAbilityComponent = NewObject<UBattlerAbilityComponent>();

    CREATE_MOCK(IBattler, User, FMockBattler, MockUser);
    CREATE_MOCK(IBattleMove, Move, FMockBattleMove, MockMove);
    CREATE_MOCK(IBattler, Target1, FMockBattler, MockTarget1);
    CREATE_MOCK(IBattler, Target2, FMockBattler, MockTarget2);

    ON_CALL(MockUser, GetAbilityComponent).WillByDefault(Return(FakeAbilityComponent));
    ON_CALL(MockUser, GetNickname).WillByDefault(Return(FText::FromStringView(TEXT("Mock User"))));
    ON_CALL(MockTarget1, IsFainted).WillByDefault(Return(true));
    ON_CALL(MockTarget2, IsFainted).WillByDefault(Return(false));

    FBattleActionUseMove Action(User, Move, {Target1, Target2});
    UE_CHECK_EQUAL(TEXT("FBattleActionUseMove"), Action.GetReferencerName());
    UE_CHECK_EQUAL(1, Action.GetPriority());
    UE_CHECK_EQUAL(TEXT("Mock User used Test Move!"), Action.GetActionMessage().ToString());

    UE_CHECK_FALSE(Action.IsExecuting());
    Action.Execute();
    UE_CHECK_TRUE(Action.IsExecuting());
    UE_CHECK_EQUAL(1, Move->CostsPaid);

    auto &Result = Action.GetActionResult();
    Result.WaitFor(FTimespan::FromMinutes(1));
    UE_ASSERT_TRUE(Result.IsReady());

    auto &ResultValue = Result.Get();
    UE_ASSERT_EQUAL(1, ResultValue.TargetResults.Num());
    UE_CHECK_TRUE(ResultValue.TargetResults[0].Target == Target2);
    UE_CHECK_EQUAL(20, ResultValue.TargetResults[0].Damage.Damage);

    */
    return true;
}