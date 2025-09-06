#include "Asserts.h"
#include "Misc/AutomationTest.h"
#include "Services/GameServiceSubsystem.h"
#include "Utilities/WidgetTestUtilities.h"
#include "Utils/TestService.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestGetServiceInCode, "Unit Tests.DependencyInjection.TestGetServiceInCode",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestGetServiceInCode::RunTest(const FString &Parameters)
{
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    auto &Service = UGameServiceSubsystem::GetService<UTestService>(World.Get());
    auto OtherService = UGameServiceSubsystem::StaticGetService(World.Get(), UTestService::StaticClass());
    UE_CHECK_TRUE(&Service == OtherService);
    return true;
}