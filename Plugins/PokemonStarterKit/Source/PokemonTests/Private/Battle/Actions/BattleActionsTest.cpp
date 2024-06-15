#include "Asserts.h"
#include "TestBattleMove.h"
#include "Battle/Actions/BattleActionUseMove.h"
#include "Misc/AutomationTest.h"
#include "Mocking/UnrealMock.h"
#include "Battle/Battlers/Battler.h"

using namespace fakeit;

IMPLEMENT_SIMPLE_AUTOMATION_TEST(BattleActionsTest_Moves, "Unit Tests.Battle.Actions.BattleActionsTest.Moves",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool BattleActionsTest_Moves::RunTest(const FString &Parameters) {
    auto [User, MockUser] = UnrealMock::CreateMock<IBattler>();
    auto Move = NewObject<UTestBattleMove>();
    auto [Target1, MockTarget1] = UnrealMock::CreateMock<IBattler>();
    auto [Target2, MockTarget2] = UnrealMock::CreateMock<IBattler>();

    When(Method(MockUser, GetNickname)).AlwaysReturn(FText::FromStringView(TEXT("Mock User")));
    When(Method(MockTarget1, IsFainted)).AlwaysReturn(true);
    When(Method(MockTarget2, IsFainted)).AlwaysReturn(false);

    FBattleActionUseMove Action(User, Move, { Target1, Target2 });
    CHECK_EQUAL(TEXT("FBattleActionUseMove"), Action.GetReferencerName());
    CHECK_EQUAL(1, Action.GetPriority());
    CHECK_EQUAL(TEXT("Mock User used Test Move!"), Action.GetActionMessage().ToString());

    CHECK_FALSE(Action.IsExecuting());
    Action.Execute();
    CHECK_TRUE(Action.IsExecuting());
    CHECK_EQUAL(1, Move->CostsPaid);

    
    auto &Result = Action.GetActionResult();
    Result.WaitFor(FTimespan::FromMinutes(1));
    ASSERT_TRUE(Result.IsReady());

    auto &ResultValue = Result.Get();
    ASSERT_EQUAL(1, ResultValue.TargetResults.Num());
    CHECK_TRUE(ResultValue.TargetResults[0].Target == Target2);
    CHECK_EQUAL(20, ResultValue.TargetResults[0].Damage.Damage);
    
    return true;
}