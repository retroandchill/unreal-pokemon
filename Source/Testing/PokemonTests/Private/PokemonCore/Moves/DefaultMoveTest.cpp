// "Unreal Pokémon" created by Retro & Chill.
#include "CommonDefines.h"
#include "Misc/AutomationTest.h"
#include "Moves/DefaultMove.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(DefaultMoveTest, "Project.Core.Moves.DefaultMoveTest",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool DefaultMoveTest::RunTest(const FString& Parameters) {
	auto Move = NewObject<UDefaultMove>()->Initialize(TEXT("SWORDSDANCE"));

	AUTOMATION_ASSERT(TestEqual(TEXT(""), Move->GetCurrentPP(), 20));
	AUTOMATION_ASSERT(TestEqual(TEXT(""), Move->GetTotalPP(), 20));
	
	return true;
}
