#include "Asserts.h"
#include "DataManager.h"
#include "Misc/AutomationTest.h"
#include "Species/Stat.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestGetMainBattleStatsFromTable, "Unit Tests.Editor.TestGetMainBattleStatsFromTable",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestGetMainBattleStatsFromTable::RunTest(const FString &Parameters) {
    auto Stats = UStatHelper::GetMainBattleStatNames();

    UE_ASSERT_EQUAL(Stats.Num(), 5);
    UE_CHECK_EQUAL(TEXT("ATTACK"), Stats[0].ToString());
    UE_CHECK_EQUAL(TEXT("DEFENSE"), Stats[1].ToString());
    UE_CHECK_EQUAL(TEXT("SPECIAL_ATTACK"), Stats[2].ToString());
    UE_CHECK_EQUAL(TEXT("SPECIAL_DEFENSE"), Stats[3].ToString());
    UE_CHECK_EQUAL(TEXT("SPEED"), Stats[4].ToString());

    return true;
}