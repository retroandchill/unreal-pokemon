#include "Asserts.h"
#include "Battle/Battlers/Battler.h"
#include "Battle/Moves/MoveEvaluationHelpers.h"
#include "Battle/Moves/SecondaryEffects.h"
#include "Misc/AutomationTest.h"
#include "Mocking/UnrealMock.h"
#include "Mocks/MockBattler.h"

using namespace testing;

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestMoveStatChanges_NoExisting, "Unit Tests.Battle.Moves.TestMoveStatChanges.NoExistingChanges",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestMoveStatChanges_NoExisting::RunTest(const FString &Parameters) {
    CREATE_MOCK(IBattler, Battler, FMockBattler, MockBattler);
    ON_CALL(MockBattler, GetStatStage).WillByDefault(Return(0));

    FAdditionalMoveEffects MoveEffects;
    UMoveEvaluationHelpers::AlterStatStages(Battler, MoveEffects, {"ATTACK"}, 2);
    UE_ASSERT_TRUE(MoveEffects.StatStageChanges.Contains("ATTACK"));
    UE_CHECK_EQUAL(2, MoveEffects.StatStageChanges["ATTACK"]);
    
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestMoveStatChanges_TooHigh, "Unit Tests.Battle.Moves.TestMoveStatChanges.TooHigh",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestMoveStatChanges_TooHigh::RunTest(const FString &Parameters) {
    CREATE_MOCK(IBattler, Battler, FMockBattler, MockBattler);
    ON_CALL(MockBattler, GetStatStage).WillByDefault(Return(5));

    FAdditionalMoveEffects MoveEffects;
    UMoveEvaluationHelpers::AlterStatStages(Battler, MoveEffects, {"ATTACK"}, 2);
    UE_ASSERT_TRUE(MoveEffects.StatStageChanges.Contains("ATTACK"));
    UE_CHECK_EQUAL(1, MoveEffects.StatStageChanges["ATTACK"]);
    
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestMoveStatChanges_TooLow, "Unit Tests.Battle.Moves.TestMoveStatChanges.TooLow",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestMoveStatChanges_TooLow::RunTest(const FString &Parameters) {
    CREATE_MOCK(IBattler, Battler, FMockBattler, MockBattler);
    ON_CALL(MockBattler, GetStatStage).WillByDefault(Return(-5));

    FAdditionalMoveEffects MoveEffects;
    UMoveEvaluationHelpers::AlterStatStages(Battler, MoveEffects, {"ATTACK"}, -2);
    UE_ASSERT_TRUE(MoveEffects.StatStageChanges.Contains("ATTACK"));
    UE_CHECK_EQUAL(-1, MoveEffects.StatStageChanges["ATTACK"]);
    
    return true;
}