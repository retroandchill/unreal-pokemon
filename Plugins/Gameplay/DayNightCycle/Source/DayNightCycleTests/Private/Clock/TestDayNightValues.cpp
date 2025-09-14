#include "Asserts.h"
#include "Clock/GameClockSubsystem.h"
#include "Clock/TickBasedClock.h"
#include "DayNightCycleSettings.h"
#include "Misc/AutomationTest.h"
#include "Utilities/ReflectionUtils.h"
#include "Utilities/WidgetTestUtilities.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestDayNightValues, "Unit Tests.Clock.TestDayNightValues",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestDayNightValues::RunTest(const FString &Parameters)
{
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    auto Subsystem = GameInstance->GetSubsystem<UGameClockSubsystem>();
    UE_ASSERT_NOT_NULL(Subsystem);

    auto Clock = NewObject<UTickBasedClock>();
    UReflectionUtils::SetPropertyValue<TScriptInterface<IGameClock>>(Subsystem, "GameClock", Clock);
    UE_ASSERT_TRUE(Subsystem->GetGameClock() == Clock);

    auto &DayLength = GetDefault<UDayNightCycleSettings>()->DayRange;
    Clock->SetCurrentTime(FDateTime(2000, 1, 1, static_cast<int32>(DayLength.GetLowerBoundValue()) - 1));

    UE_ASSERT_FALSE(Subsystem->IsDay());
    UE_ASSERT_EQUAL(0.f, Subsystem->GetDayCoefficient());

    Clock->SetCurrentTime(FDateTime(2000, 1, 1, static_cast<int32>(DayLength.GetUpperBoundValue()) - 1));
    UE_ASSERT_TRUE(Subsystem->IsDay());
    UE_ASSERT_TRUE(Subsystem->GetDayCoefficient() >= 0.f);
    UE_ASSERT_TRUE(Subsystem->GetDayCoefficient() <= 1.f);

    return true;
}