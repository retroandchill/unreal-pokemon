// "Unreal Pokémon" created by Retro & Chill.
#include "MathUtilities.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LinearInterpolationTest, "Project.Math.LinearInterpolationTest",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool LinearInterpolationTest::RunTest(const FString& Parameters) {
	// Make the test pass by returning true, or fail by returning false.
	bool Passed = TestEqual("Negative Duration", 70.0, UMathUtilities::LinearInterpolation(30.0, 70, -3, 20));
	Passed &= TestEqual("Negative Delta", 30.0f, UMathUtilities::LinearInterpolationF(30.f, 70.f, 20.f, -8.f));
	Passed &= TestEqual("Delta Past Duration", 70.0, UMathUtilities::LinearInterpolation(30.0, 70, 20, 30));
	Passed &= TestEqual("Delta Within Duration", 50.0, UMathUtilities::LinearInterpolation(30.0, 70, 20, 10));
	return Passed;
}
