#include "Asserts.h"
#include "Clock/TickBasedClock.h"
#include "Misc/AutomationTest.h"
#include "Utilities/ReflectionUtils.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestTickBasedClock, "Unit Tests.Clock.TestTickBasedClock",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestTickBasedClock::RunTest(const FString &Parameters) {
    auto Clock = NewObject<UTickBasedClock>();
    auto StartTime = IGameClock::Execute_GetCurrentTime(Clock);
    auto DayLength = UReflectionUtils::GetPropertyValue<float>(Clock, "DayLength");
    Clock->Tick(DayLength / 24.f); // Elapse an hour
    auto CurrentTime = IGameClock::Execute_GetCurrentTime(Clock);
    UE_ASSERT_EQUAL(FTimespan::FromHours(1), (CurrentTime - StartTime));
    return true;
}