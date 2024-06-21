// "Unreal Pokémon" created by Retro & Chill.

#include "Asserts.h"
#include "Bag/Item.h"
#include "BlueprintActionDatabase.h"
#include "BlueprintTypePromotion.h"
#include "Data/K2Node_GetGameData.h"
#include "DataManager.h"
#include "Dispatchers/TestDispatcher.h"
#include "Exp/GrowthRateData.h"
#include "GraphEditorSettings.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "Misc/AutomationTest.h"
#include "Species/EggGroup.h"
#include "Species/SpeciesData.h"
#include "Species/Stat.h"
#include "Utilities/BlueprintTestUtils.h"
#include "Utilities/K2Nodes.h"
#include "Utilities/ReflectionUtils.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestGetGameData_NodeTile,
                                 "Unit Tests.PokemonData.Nodes.TestGetGameData.NodeTitle",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestGetGameData_NodeTile::RunTest(const FString &Parameters) {
    using enum ENodeTitleType::Type;
    FBlueprintActionDatabase::Get().RefreshAll();

    MakeTestableBP(TestBP, TestGraph);

    auto TestNode = NewObject<UK2Node_GetGameData>(TestGraph.Get());
    UE_ASSERT_EQUAL(TEXT("Invalid Struct Type"), TestNode->GetNodeTitle(MenuTitle).ToString());
    TestNode->Initialize(FGrowthRateData::StaticStruct());
    TestGraph->AddNode(TestNode);

    UE_ASSERT_EQUAL(TEXT("Get Growth Rate Data"), TestNode->GetNodeTitle(MenuTitle).ToString());
    UE_ASSERT_EQUAL(TEXT("Growth Rate Data"), TestNode->GetNodeTitle(FullTitle).ToString());
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestGetGameData_TooltipText,
                                 "Unit Tests.PokemonData.Nodes.TestGetGameData.TooltipText",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestGetGameData_TooltipText::RunTest(const FString &Parameters) {
    FBlueprintActionDatabase::Get().RefreshAll();

    MakeTestableBP(TestBP, TestGraph);

    auto TestNode = NewObject<UK2Node_GetGameData>(TestGraph.Get());
    UE_ASSERT_EQUAL(TEXT("Invalid Struct Type"), TestNode->GetTooltipText().ToString());
    TestNode->Initialize(FStat::StaticStruct());
    TestGraph->AddNode(TestNode);

    UE_ASSERT_EQUAL(TEXT("Get Stat \n\nRepresents one of the stats in the database."),
                 TestNode->GetTooltipText().ToString());

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestGetGameData_MenuCategory,
                                 "Unit Tests.PokemonData.Nodes.TestGetGameData.MenuCategory",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestGetGameData_MenuCategory::RunTest(const FString &Parameters) {
    FBlueprintActionDatabase::Get().RefreshAll();

    MakeTestableBP(TestBP, TestGraph);

    auto TestNode = NewObject<UK2Node_GetGameData>(TestGraph.Get());
    UE_ASSERT_EQUAL(FText::GetEmpty().ToString(), TestNode->GetMenuCategory().ToString());
    TestNode->Initialize(FSpeciesData::StaticStruct());
    TestGraph->AddNode(TestNode);

    UE_ASSERT_EQUAL(TEXT("Data|PBS|Species Data"), TestNode->GetMenuCategory().ToString());

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestGetGameData_IconAndTint,
                                 "Unit Tests.PokemonData.Nodes.TestGetGameData.IconAndTint",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestGetGameData_IconAndTint::RunTest(const FString &Parameters) {
    FBlueprintActionDatabase::Get().RefreshAll();

    MakeTestableBP(TestBP, TestGraph);

    auto TestNode = NewObject<UK2Node_GetGameData>(TestGraph.Get());
    TestNode->Initialize(FItem::StaticStruct());
    TestGraph->AddNode(TestNode);

    FLinearColor Color;
    auto Icon = TestNode->GetIconAndTint(Color);

    UE_ASSERT_EQUAL(GetDefault<UGraphEditorSettings>()->PureFunctionCallNodeTitleColor, Color);
    UE_ASSERT_EQUAL(FSlateIcon(FAppStyle::GetAppStyleSetName(), "Kismet.AllClasses.FunctionIcon"), Icon);

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestGetGameData_MenuActions,
                                 "Unit Tests.PokemonData.Nodes.TestGetGameData.MenuActions",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestGetGameData_MenuActions::RunTest(const FString &Parameters) {
    FBlueprintActionDatabase::Get().RefreshAll();
    auto Actions = FBlueprintActionDatabase::Get().GetAllActions();
    UE_ASSERT_TRUE(Actions.Contains(UK2Node_GetGameData::StaticClass()));
    auto NodeActions = Actions[UK2Node_GetGameData::StaticClass()];
    UE_ASSERT_EQUAL(FDataManager::GetInstance().GetStructTypes().Num(), NodeActions.Num());

    return true;
}

constexpr auto TEST_GET_GAME_DATA = TEXT("/PokemonStarterKit/Tests/Resources/GetDataDispatcher.GetDataDispatcher");

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestGetGameData_ExecuteNode,
                                 "Unit Tests.PokemonData.Nodes.TestGetGameData.NodeExecution",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestGetGameData_ExecuteNode::RunTest(const FString &Parameters) {
    auto DispatcherClass = UBlueprintTestUtils::LoadBlueprintClassByName(TEST_GET_GAME_DATA);
    UE_ASSERT_NOT_NULL(DispatcherClass);
    auto Dispatcher = NewObject<UObject>(GEngine, DispatcherClass);
    ITestDispatcher::Execute_ExecuteTest(Dispatcher);
    auto &EggGroup = UReflectionUtils::GetPropertyValue<FEggGroup>(Dispatcher, TEXT("EggGroup"));
    UE_CHECK_EQUAL(TEXT("FIELD"), EggGroup.ID.ToString().ToUpper());
    UE_CHECK_EQUAL(TEXT("Field"), EggGroup.RealName.ToString());
    UE_CHECK_EQUAL(EEggGroupType::Normal, EggGroup.Type);

    return true;
}