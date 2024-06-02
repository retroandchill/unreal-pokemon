#include "Asserts.h"
#include "MathUtilities.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LinearInterpolationTest, "Unit Tests.Math.LinearInterpolationTest",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool LinearInterpolationTest::RunTest(const FString &Parameters) {
    CHECK_EQUAL(70.0, UMathUtilities::LinearInterpolation(30.0, 70, -3, 20));
    CHECK_EQUAL(30.0f, UMathUtilities::LinearInterpolation(30.f, 70.f, 20.f, -8.f));
    CHECK_EQUAL(70.0, UMathUtilities::LinearInterpolation(30.0, 70, 20, 30));
    CHECK_EQUAL(50.0, UMathUtilities::LinearInterpolation(30.0, 70, 20, 10));
    return true;
}