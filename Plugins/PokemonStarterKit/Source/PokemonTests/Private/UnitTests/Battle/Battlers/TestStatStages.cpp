#include "Asserts.h"
#include "Battle/Battlers/BattlerStats.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestStatStages, "Unit Tests.Battle.Battlers.TestStatStages",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestStatStages::RunTest(const FString &Parameters) {
    FMainBattleStat Stat = { .StatValue = 100, .Stages = 0 };
    UE_CHECK_EQUAL(100, Stat.GetModifiedValue());

    Stat.Stages = 1;
    UE_CHECK_EQUAL(150, Stat.GetModifiedValue());
    Stat.Stages = 2;
    UE_CHECK_EQUAL(200, Stat.GetModifiedValue());    
    Stat.Stages = 3;
    UE_CHECK_EQUAL(250, Stat.GetModifiedValue());
    Stat.Stages = 4;
    UE_CHECK_EQUAL(300, Stat.GetModifiedValue());
    Stat.Stages = 5;
    UE_CHECK_EQUAL(350, Stat.GetModifiedValue());
    Stat.Stages = 6;
    UE_CHECK_EQUAL(400, Stat.GetModifiedValue());

    Stat.Stages = -1;
    UE_CHECK_EQUAL(66, Stat.GetModifiedValue());
    Stat.Stages = -2;
    UE_CHECK_EQUAL(50, Stat.GetModifiedValue());    
    Stat.Stages = -3;
    UE_CHECK_EQUAL(40, Stat.GetModifiedValue());
    Stat.Stages = -4;
    UE_CHECK_EQUAL(33, Stat.GetModifiedValue());
    Stat.Stages = -5;
    UE_CHECK_EQUAL(28, Stat.GetModifiedValue());
    Stat.Stages = -6;
    UE_CHECK_EQUAL(25, Stat.GetModifiedValue());
    
    return true;
}