#include "Asserts.h"
#include "Clock/RealTimeClock.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestRealTimeClock, "Unit Tests.Clock.TestRealTimeClock",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestRealTimeClock::RunTest(const FString &Parameters) {
    auto Clock = NewObject<URealTimeClock>();
    UE_ASSERT_EQUAL(FDateTime::Now(), IGameClock::Execute_GetCurrentTime(Clock));
    return true;
}