#include "Asserts.h"
#include "Misc/AutomationTest.h"
#include "Services/GameServiceSubsystem.h"
#include "Utilities/WidgetTestUtilities.h"
#include "Utils/TestService.h"
#include "Utils/TestServiceBlueprintNodeExecutor.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestGetServiceInBlueprint, "Unit Tests.DependencyInjection.TestGetServiceInBlueprint",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestGetServiceInBlueprint::RunTest(const FString &Parameters)
{
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    auto UtilityClass = LoadClass<UObject>(
        nullptr, TEXT("/UnrealInjector/Tests/Resources/TestContextTestObject.TestContextTestObject_C"));
    UE_ASSERT_NOT_NULL(UtilityClass);
    UE_ASSERT_TRUE(UtilityClass->ImplementsInterface(UTestServiceBlueprintNodeExecutor::StaticClass()));
    auto Object = NewObject<UObject>(World.Get(), UtilityClass);
    ITestServiceBlueprintNodeExecutor::Execute_SetTestValue(Object, World.Get(), 64);
    auto &Service = UGameServiceSubsystem::GetService<UTestService>(World.Get());
    UE_CHECK_EQUAL(64, Service.TestValue);
    return true;
}