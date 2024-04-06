// "Unreal Pokémon" created by Retro & Chill.
#include "BlueprintActionDatabase.h"
#include "CommonDefines.h"
#include "EditorCategoryUtils.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "Misc/AutomationTest.h"
#include "Nodes/K2Node_AddWidgetToStack.h"
#include "Screens/PauseMenuScreen.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestAddWidgetToStack, "UnrealPokemon.RPGMenus.TestAddWidgetToStack.NodeInfo",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestAddWidgetToStack::RunTest(const FString& Parameters) {
	using enum ENodeTitleType::Type;
	FBlueprintActionDatabase::Get().RefreshAll();

	MakeTestableBP(TestBP, TestGraph);

	auto TestNode = NewObject<UK2Node_AddWidgetToStack>(TestGraph.Get());
	TestGraph->AddNode(TestNode);

	AUTOMATION_ASSERT(TestEqual(TEXT("Menu Category"),
		TestNode->GetMenuCategory().ToString(),
		FEditorCategoryUtils::GetCommonCategory(FCommonEditorCategory::UserInterface).ToString()));

	AUTOMATION_ASSERT(TestEqual(TEXT("Corner Icon"),
		TestNode->GetCornerIcon().ToString(),
		TEXT("Graph.Replication.ClientEvent")));

	AUTOMATION_ASSERT(TestEqual(TEXT("Node Title (Screen Note Known)"),
		TestNode->GetNodeTitle(MenuTitle).ToString(),
		TEXT("Add Screen to Stack")));

	AUTOMATION_ASSERT(TestEqual(TEXT("Node Title (Screen Note Known)"),
		TestNode->GetNodeTitle(FullTitle).ToString(),
		TEXT("Add NONE to Stack")));

	TestNode->AllocateDefaultPins();
	auto StructPin = TestNode->GetPinAt(2);
	StructPin->DefaultObject = UPauseMenuScreen::StaticClass();
	AUTOMATION_ASSERT(TestEqual(TEXT("Node Title (Screen Note Known)"),
		TestNode->GetNodeTitle(FullTitle).ToString(),
		TEXT("Add Pause Menu Screen to Stack")));
	
	return true;
}
