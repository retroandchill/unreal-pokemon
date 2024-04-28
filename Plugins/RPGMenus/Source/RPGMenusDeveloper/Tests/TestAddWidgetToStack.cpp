// "Unreal Pokémon" created by Retro & Chill.

#if WITH_TESTS && HAS_AUTOMATION_HELPERS
#include "Asserts.h"
#include "BlueprintActionDatabase.h"
#include "EditorCategoryUtils.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "Misc/AutomationTest.h"
#include "Nodes/K2Node_AddWidgetToStack.h"
#include "Utilities/BlueprintTestUtils.h"
#include "Utilities/K2Nodes.h"

constexpr auto TEST_SCREEN = TEXT("/RPGMenus/Tests/Resources/TestScreen.TestScreen");

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestAddWidgetToStack, "Unit Tests.RPGMenus.TestAddWidgetToStack.NodeInfo",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestAddWidgetToStack::RunTest(const FString &Parameters) {
    using enum ENodeTitleType::Type;
    FBlueprintActionDatabase::Get().RefreshAll();

    MakeTestableBP(TestBP, TestGraph);

    auto TestNode = NewObject<UK2Node_AddWidgetToStack>(TestGraph.Get());
    TestGraph->AddNode(TestNode);

    CHECK_EQUAL(FEditorCategoryUtils::GetCommonCategory(FCommonEditorCategory::UserInterface).ToString(),
                TestNode->GetMenuCategory().ToString());
    CHECK_EQUAL(TestNode->GetCornerIcon().ToString(), TEXT("Graph.Replication.ClientEvent"));
    CHECK_EQUAL(TEXT("Add Screen to Stack"), TestNode->GetNodeTitle(MenuTitle).ToString());
    CHECK_EQUAL(TEXT("Add NONE to Stack"), TestNode->GetNodeTitle(FullTitle).ToString());

    TestNode->AllocateDefaultPins();
    auto StructPin = TestNode->GetPinAt(2);
    StructPin->DefaultObject = UBlueprintTestUtils::LoadBlueprintClassByName(TEST_SCREEN);
    CHECK_EQUAL(TEXT("Add Test Screen to Stack"), TestNode->GetNodeTitle(FullTitle).ToString());

    return true;
}
#endif