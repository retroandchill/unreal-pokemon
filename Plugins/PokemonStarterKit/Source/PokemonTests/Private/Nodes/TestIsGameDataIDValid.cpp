// "Unreal Pokémon" created by Retro & Chill.

#include "Asserts.h"
#include "Bag/Item.h"
#include "BlueprintActionDatabase.h"
#include "DataManager.h"
#include "Dispatchers/TestDispatcher.h"
#include "Exp/GrowthRateData.h"
#include "GraphEditorSettings.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "Misc/AutomationTest.h"
#include "Data/K2Node_IsGameDataIDValid.h"
#include "Species/SpeciesData.h"
#include "Species/Stat.h"
#include "Utilities/BlueprintTestUtils.h"
#include "Utilities/K2Nodes.h"
#include "Utilities/ReflectionUtils.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestIsGameDataIDValid_NodeTile,
                                 "Unit Tests.PokemonData.Nodes.TestIsGameDataIDValid.NodeTitle",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestIsGameDataIDValid_NodeTile::RunTest(const FString &Parameters) {
    using enum ENodeTitleType::Type;
    FBlueprintActionDatabase::Get().RefreshAll();

    MakeTestableBP(TestBP, TestGraph);

    auto TestNode = NewObject<UK2Node_IsGameDataIDValid>(TestGraph.Get());
    ASSERT_EQUAL(TEXT("Invalid Struct Type"), TestNode->GetNodeTitle(MenuTitle).ToString());
    TestNode->Initialize(FGrowthRateData::StaticStruct());
    TestGraph->AddNode(TestNode);

    ASSERT_EQUAL(TEXT("Is Growth Rate Data ID Valid?"), TestNode->GetNodeTitle(MenuTitle).ToString());
    ASSERT_EQUAL(TEXT("Growth Rate Data"), TestNode->GetNodeTitle(FullTitle).ToString());
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestIsGameDataIDValid_TooltipText,
                                 "Unit Tests.PokemonData.Nodes.TestIsGameDataIDValid.TooltipText",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestIsGameDataIDValid_TooltipText::RunTest(const FString &Parameters) {
    FBlueprintActionDatabase::Get().RefreshAll();

    MakeTestableBP(TestBP, TestGraph);

    auto TestNode = NewObject<UK2Node_IsGameDataIDValid>(TestGraph.Get());
    ASSERT_EQUAL(TEXT("Invalid Struct Type"), TestNode->GetTooltipText().ToString());
    TestNode->Initialize(FStat::StaticStruct());
    TestGraph->AddNode(TestNode);

    ASSERT_EQUAL(TEXT("Is Stat ID Valid? \n\nRepresents one of the stats in the database."),
                 TestNode->GetTooltipText().ToString());

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestIsGameDataIDValid_MenuCategory,
                                 "Unit Tests.PokemonData.Nodes.TestIsGameDataIDValid.MenuCategory",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestIsGameDataIDValid_MenuCategory::RunTest(const FString &Parameters) {
    FBlueprintActionDatabase::Get().RefreshAll();

    MakeTestableBP(TestBP, TestGraph);

    auto TestNode = NewObject<UK2Node_IsGameDataIDValid>(TestGraph.Get());
    ASSERT_EQUAL(FText::GetEmpty().ToString(), TestNode->GetMenuCategory().ToString());
    TestNode->Initialize(FSpeciesData::StaticStruct());
    TestGraph->AddNode(TestNode);

    ASSERT_EQUAL(TEXT("Data|PBS|Species Data"), TestNode->GetMenuCategory().ToString());

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestIsGameDataIDValid_IconAndTint,
                                 "Unit Tests.PokemonData.Nodes.TestIsGameDataIDValid.IconAndTint",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestIsGameDataIDValid_IconAndTint::RunTest(const FString &Parameters) {
    FBlueprintActionDatabase::Get().RefreshAll();

    MakeTestableBP(TestBP, TestGraph);

    auto TestNode = NewObject<UK2Node_IsGameDataIDValid>(TestGraph.Get());
    TestNode->Initialize(FItem::StaticStruct());
    TestGraph->AddNode(TestNode);

    FLinearColor Color;
    auto Icon = TestNode->GetIconAndTint(Color);

    ASSERT_EQUAL(GetDefault<UGraphEditorSettings>()->PureFunctionCallNodeTitleColor, Color);
    ASSERT_EQUAL(FSlateIcon(FAppStyle::GetAppStyleSetName(), "Kismet.AllClasses.FunctionIcon"), Icon);

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestIsGameDataIDValid_MenuActions,
                                 "Unit Tests.PokemonData.Nodes.TestIsGameDataIDValid.MenuActions",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestIsGameDataIDValid_MenuActions::RunTest(const FString &Parameters) {
    FBlueprintActionDatabase::Get().RefreshAll();
    auto Actions = FBlueprintActionDatabase::Get().GetAllActions();
    ASSERT_TRUE(Actions.Contains(UK2Node_IsGameDataIDValid::StaticClass()));
    auto NodeActions = Actions[UK2Node_IsGameDataIDValid::StaticClass()];
    ASSERT_EQUAL(FDataManager::GetInstance().GetStructTypes().Num(), NodeActions.Num());

    return true;
}

constexpr auto TEST_ID_GAME_DATA_ID_VALID =
    TEXT("/PokemonData/Tests/Resources/IsGameDataIdValidDispatcher.IsGameDataIdValidDispatcher");

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestIsGameDataIDValid_ExecuteNode,
                                 "Unit Tests.PokemonData.Nodes.TestIsGameDataIDValid.NodeExecution",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestIsGameDataIDValid_ExecuteNode::RunTest(const FString &Parameters) {
    auto DispatcherClass = UBlueprintTestUtils::LoadBlueprintClassByName(TEST_ID_GAME_DATA_ID_VALID);
    ASSERT_NOT_NULL(DispatcherClass);
    auto Dispatcher = NewObject<UObject>(GEngine, DispatcherClass);
    ITestDispatcher::Execute_ExecuteTest(Dispatcher);
    bool ValidName = UReflectionUtils::GetPropertyValue<bool>(Dispatcher, TEXT("ValidName"));
    bool InvalidName = UReflectionUtils::GetPropertyValue<bool>(Dispatcher, TEXT("InvalidName"));
    CHECK_TRUE(ValidName);
    CHECK_FALSE(InvalidName);

    return true;
}