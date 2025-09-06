#include "Asserts.h"
#include "Characters/FacingDirection.h"
#include "GridUtils.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(GetOpposingDirectionTest, "Unit Tests.GridBased2D.GetOpposingDirectionTest",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool GetOpposingDirectionTest::RunTest(const FString &Parameters)
{
    UE_ASSERT_EQUAL(EFacingDirection::Up, UGridUtils::GetOpposingDirection(EFacingDirection::Down));
    UE_ASSERT_EQUAL(EFacingDirection::Down, UGridUtils::GetOpposingDirection(EFacingDirection::Up));
    UE_ASSERT_EQUAL(EFacingDirection::Left, UGridUtils::GetOpposingDirection(EFacingDirection::Right));
    UE_ASSERT_EQUAL(EFacingDirection::Right, UGridUtils::GetOpposingDirection(EFacingDirection::Left));
    return true;
}