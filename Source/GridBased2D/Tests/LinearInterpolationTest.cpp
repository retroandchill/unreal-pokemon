#include "GridUtils.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LinearInterpolationTest, "Tests.LinearInterpolationTest",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool LinearInterpolationTest::RunTest(const FString& Parameters) {
	// Make the test pass by returning true, or fail by returning false.
	bool Passed = TestEqual("Negative Duration", 70.0, GridBased2D::LinearInterpolation(30, 70, -3, 20));
	Passed &= TestEqual("Negative Delta", 30.0, GridBased2D::LinearInterpolation(30, 70, 20, -8));
	Passed &= TestEqual("Delta Past Duration", 70.0, GridBased2D::LinearInterpolation(30, 70, 20, 30));
	Passed &= TestEqual("Delta Within Duration", 50.0, GridBased2D::LinearInterpolation(30, 70, 20, 10));
	return Passed;
}
