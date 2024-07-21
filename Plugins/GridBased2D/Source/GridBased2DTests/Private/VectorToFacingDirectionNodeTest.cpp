#include "Asserts.h"
#include "GridUtils.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(VectorToFacingDirectionNodeTest, "Unit Tests.GridBased2D.VectorToFacingDirectionNodeTest",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool VectorToFacingDirectionNodeTest::RunTest(const FString &Parameters) {
    EValidDirection IsValid;
    EFacingDirection Direction;

    UGridUtils::VectorToFacingDirection(FVector2D(0, 0), Direction, IsValid);
    UE_ASSERT_EQUAL(EValidDirection::NoDirection, IsValid);

    UGridUtils::VectorToFacingDirection(FVector2D(1, 0), Direction, IsValid);
    UE_ASSERT_EQUAL(EValidDirection::HasDirection, IsValid);
    UE_ASSERT_EQUAL(EFacingDirection::Right, Direction);
    
    return true;
}