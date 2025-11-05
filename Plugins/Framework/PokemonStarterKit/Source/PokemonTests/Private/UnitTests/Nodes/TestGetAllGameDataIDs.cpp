// "Unreal Pokémon" created by Retro & Chill.

#include "Asserts.h"
#include "Bag/Item.h"
#include "BlueprintActionDatabase.h"
#include "Data/K2Node_GetAllGameDataIDs.h"
#include "DataManager.h"
#include "Dispatchers/TestDispatcher.h"
#include "Engine/Blueprint.h"
#include "Exp/GrowthRateData.h"
#include "GraphEditorSettings.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "Misc/AutomationTest.h"
#include "Species/SpeciesData.h"
#include "Species/Stat.h"
#include "Utilities/BlueprintTestUtils.h"
#include "Utilities/K2Nodes.h"
#include "Utilities/ReflectionUtils.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestGetAllGameDataIDs_NodeTile,
                                 "Unit Tests.PokemonData.Nodes.TestGetAllGameDataIDs.NodeTitle",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestGetAllGameDataIDs_NodeTile::RunTest(const FString &Parameters) {
    using enum ENodeTitleType::Type;
    FBlueprintActionDatabase::Get().RefreshAll();

    MakeTestableBP(TestBP, TestGraph);

    auto TestNode = NewObject<UK2Node_GetAllGameDataIDs>(TestGraph.Get());
    UE_ASSERT_EQUAL(TEXT("Invalid Struct Type"), TestNode->GetNodeTitle(MenuTitle).ToString());
    TestNode->Initialize(FGrowthRateData::StaticStruct());
    TestGraph->AddNode(TestNode);

    UE_ASSERT_EQUAL(TEXT("Get All Growth Rate Data IDs"), TestNode->GetNodeTitle(MenuTitle).ToString());
    UE_ASSERT_EQUAL(TEXT("Growth Rate Data"), TestNode->GetNodeTitle(FullTitle).ToString());

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestGetAllGameDataIDs_TooltipText,
                                 "Unit Tests.PokemonData.Nodes.TestGetAllGameDataIDs.TooltipText",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestGetAllGameDataIDs_TooltipText::RunTest(const FString &Parameters) {
    FBlueprintActionDatabase::Get().RefreshAll();

    MakeTestableBP(TestBP, TestGraph);

    auto TestNode = NewObject<UK2Node_GetAllGameDataIDs>(TestGraph.Get());
    UE_ASSERT_EQUAL(TEXT("Invalid Struct Type"), TestNode->GetTooltipText().ToString());
    TestNode->Initialize(FStat::StaticStruct());
    TestGraph->AddNode(TestNode);

    UE_ASSERT_EQUAL(TEXT("Get All Stat IDs \n\nRepresents one of the stats in the database."),
                    TestNode->GetTooltipText().ToString());

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestGetAllGameDataIDs_MenuCategory,
                                 "Unit Tests.PokemonData.Nodes.TestGetAllGameDataIDs.MenuCategory",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestGetAllGameDataIDs_MenuCategory::RunTest(const FString &Parameters) {
    FBlueprintActionDatabase::Get().RefreshAll();

    MakeTestableBP(TestBP, TestGraph);

    auto TestNode = NewObject<UK2Node_GetAllGameDataIDs>(TestGraph.Get());
    UE_ASSERT_EQUAL(FText::GetEmpty().ToString(), TestNode->GetMenuCategory().ToString());
    TestNode->Initialize(FSpeciesData::StaticStruct());
    TestGraph->AddNode(TestNode);

    UE_ASSERT_EQUAL(TEXT("Data|PBS|Species Data"), TestNode->GetMenuCategory().ToString());

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestGetAllGameDataIDs_IconAndTint,
                                 "Unit Tests.PokemonData.Nodes.TestGetAllGameDataIDs.IconAndTint",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestGetAllGameDataIDs_IconAndTint::RunTest(const FString &Parameters) {
    FBlueprintActionDatabase::Get().RefreshAll();

    MakeTestableBP(TestBP, TestGraph);

    auto TestNode = NewObject<UK2Node_GetAllGameDataIDs>(TestGraph.Get());
    TestNode->Initialize(FItem::StaticStruct());
    TestGraph->AddNode(TestNode);

    FLinearColor Color;
    auto Icon = TestNode->GetIconAndTint(Color);

    UE_ASSERT_EQUAL(GetDefault<UGraphEditorSettings>()->PureFunctionCallNodeTitleColor, Color);
    UE_ASSERT_EQUAL(FSlateIcon(FAppStyle::GetAppStyleSetName(), "Kismet.AllClasses.FunctionIcon"), Icon);

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestGetAllGameDataIDs_MenuActions,
                                 "Unit Tests.PokemonData.Nodes.TestGetAllGameDataIDs.MenuActions",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestGetAllGameDataIDs_MenuActions::RunTest(const FString &Parameters) {
    FBlueprintActionDatabase::Get().RefreshAll();
    auto Actions = FBlueprintActionDatabase::Get().GetAllActions();
    UE_ASSERT_TRUE(Actions.Contains(UK2Node_GetAllGameDataIDs::StaticClass()));
    auto NodeActions = Actions[UK2Node_GetAllGameDataIDs::StaticClass()];
    UE_ASSERT_EQUAL(FDataManager::GetInstance().GetStructTypes().Num(), NodeActions.Num());

    return true;
}

constexpr auto TEST_GET_GAME_DATA_IDS =
    TEXT("/PokemonStarterKit/Tests/Resources/GetDataIdsDispatcher.GetDataIdsDispatcher");

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestGetAllGameDataIDs_ExecuteNode,
                                 "Unit Tests.PokemonData.Nodes.TestGetAllGameDataIDs.NodeExecution",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestGetAllGameDataIDs_ExecuteNode::RunTest(const FString &Parameters) {
    auto DispatcherClass = UBlueprintTestUtils::LoadBlueprintClassByName(TEST_GET_GAME_DATA_IDS);
    UE_ASSERT_NOT_NULL(DispatcherClass);
    auto Dispatcher = NewObject<UObject>(GEngine, DispatcherClass);
    ITestDispatcher::Execute_ExecuteTest(Dispatcher);
    auto &GrowthRates = UReflectionUtils::GetPropertyValue<TArray<FName>>(Dispatcher, TEXT("GrowthRates"));
    UE_ASSERT_EQUAL(6, GrowthRates.Num());

    return true;
}