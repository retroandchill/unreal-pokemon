// "Unreal Pokémon" created by Retro & Chill.
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestGetAllGameDataIDs, "Project.PokemonData.Nodes.TestGetAllGameDataIDs",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestGetAllGameDataIDs::RunTest(const FString& Parameters) {
	// Make the test pass by returning true, or fail by returning false.
	return true;
}
