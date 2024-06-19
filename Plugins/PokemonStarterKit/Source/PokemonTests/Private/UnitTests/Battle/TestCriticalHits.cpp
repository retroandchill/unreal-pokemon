#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestCriticalHits, "Private.UnitTests.Battle.TestCriticalHits",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestCriticalHits::RunTest(const FString &Parameters) {
    // Make the test pass by returning true, or fail by returning false.
    FMath::RandInit(151563);

    for (int32 i = 0; i < 2; i++) {
        int32 CritRoll = FMath::Rand() % 25;
        UE_LOG(LogBlueprint, Warning, TEXT("%d: %d"), i, CritRoll);
    }
    
    return true;
}