// "Unreal Pokémon" created by Retro & Chill.
#include "Misc/AutomationTest.h"

#include <sstream>

#include "GridUtils.h"
#include "Characters/FacingDirection.h"

IMPLEMENT_COMPLEX_AUTOMATION_TEST(VectorToFacingDirectionTest, "Project.Movement.VectorToFacingDirectionTest",
                                  EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

void VectorToFacingDirectionTest::GetTests(TArray<FString>& OutBeautifiedNames,
                                           TArray<FString>& OutTestCommands) const {
	OutBeautifiedNames.Add("Up");
	OutTestCommands.Add("0 -1 " + FString::FromInt(static_cast<uint8>(EFacingDirection::Up)));
	
	OutBeautifiedNames.Add("Left");
	OutTestCommands.Add("-1 0 " + FString::FromInt(static_cast<uint8>(EFacingDirection::Left)));
	
	OutBeautifiedNames.Add("Right");
	OutTestCommands.Add("1 0 " + FString::FromInt(static_cast<uint8>(EFacingDirection::Right)));
	
	OutBeautifiedNames.Add("Down");
	OutTestCommands.Add("0 1 " + FString::FromInt(static_cast<uint8>(EFacingDirection::Down)));
}

bool VectorToFacingDirectionTest::RunTest(const FString& Parameters) {
	std::stringstream InputStream(TCHAR_TO_UTF8(*Parameters));
	double X;
	double Y;
	int32 DirValue;
	InputStream >> X >> Y >> DirValue;

	auto ExpectedDir = static_cast<EFacingDirection>(DirValue);
	auto ActualDir = GridBased2D::VectorToFacingDirection(FVector2D(X, Y));
	return TestTrue("Direction should be valid!", ActualDir.IsSet()) &&
		TestEqual("Direction should match vector!", ExpectedDir, ActualDir.GetValue());
}
