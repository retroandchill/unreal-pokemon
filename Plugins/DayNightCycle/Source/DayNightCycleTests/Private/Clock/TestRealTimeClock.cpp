#include "Clock/RealTimeClock.h"
#include "Misc/AutomationTest.h"
#include "Misc/LowLevelTestAdapter.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestRealTimeClock, "Unit Tests.Clock.TestRealTimeClock",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestRealTimeClock::RunTest(const FString &Parameters)
{
    auto Clock = NewObject<URealTimeClock>();
    auto Now = FDateTime::Now();
    CHECK(Now <= IGameClock::Execute_GetCurrentTime(Clock));
    return true;
}