#include "Asserts.h"
#include "Battle/Moves/MoveEvaluationHelpers.h"
#include "Misc/AutomationTest.h"
#include "UtilityClasses/BattleActors/CriticalTestMove.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestCritRateOverrideCriteria, "Unit Tests.TestCritRateOverrideCriteria",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestCritRateOverrideCriteria::RunTest(const FString &Parameters)
{
    using enum ECriticalOverride;
    UE_CHECK_EQUAL(Normal, UMoveEvaluationHelpers::ApplyCriticalHitOverride(Normal, Normal));
    UE_CHECK_EQUAL(Always, UMoveEvaluationHelpers::ApplyCriticalHitOverride(Normal, Always));
    UE_CHECK_EQUAL(Never, UMoveEvaluationHelpers::ApplyCriticalHitOverride(Normal, Never));

    UE_CHECK_EQUAL(Always, UMoveEvaluationHelpers::ApplyCriticalHitOverride(Always, Normal));
    UE_CHECK_EQUAL(Always, UMoveEvaluationHelpers::ApplyCriticalHitOverride(Always, Always));
    UE_CHECK_EQUAL(Never, UMoveEvaluationHelpers::ApplyCriticalHitOverride(Always, Never));

    UE_CHECK_EQUAL(Never, UMoveEvaluationHelpers::ApplyCriticalHitOverride(Never, Normal));
    UE_CHECK_EQUAL(Never, UMoveEvaluationHelpers::ApplyCriticalHitOverride(Never, Always));
    UE_CHECK_EQUAL(Never, UMoveEvaluationHelpers::ApplyCriticalHitOverride(Never, Never));

    return true;
}