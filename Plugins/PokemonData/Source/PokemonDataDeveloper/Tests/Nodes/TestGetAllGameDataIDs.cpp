// "Unreal Pokémon" created by Retro & Chill.
#include "DataManager.h"
#if WITH_TESTS && HAS_AUTOMATION_HELPERS

#include "Bag/Item.h"
#include "BlueprintActionDatabase.h"
#include "Exp/GrowthRateData.h"
#include "GraphEditorSettings.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "Misc/AutomationTest.h"
#include "Nodes/K2Node_GetAllGameDataIDs.h"
#include "Species/SpeciesData.h"
#include "Species/Stat.h"
#include "Engine/Blueprint.h"
#include "Utilities/K2Nodes.h"
#include "Asserts.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestGetAllGameDataIDs_NodeTile,
                                 "UnrealPokemon.PokemonData.Nodes.TestGetAllGameDataIDs.NodeTitle",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestGetAllGameDataIDs_NodeTile::RunTest(const FString &Parameters) {
    using enum ENodeTitleType::Type;
    FBlueprintActionDatabase::Get().RefreshAll();

    MakeTestableBP(TestBP, TestGraph);

    auto TestNode = NewObject<UK2Node_GetAllGameDataIDs>(TestGraph.Get());
    ASSERT_EQUAL(TEXT("Invalid Struct Type"), TestNode->GetNodeTitle(MenuTitle).ToString());
    TestNode->Initialize(FGrowthRateData::StaticStruct());
    TestGraph->AddNode(TestNode);

    ASSERT_EQUAL(TEXT("Get All Growth Rate Data IDs"), TestNode->GetNodeTitle(MenuTitle).ToString());
    ASSERT_EQUAL(TEXT("Growth Rate Data"), TestNode->GetNodeTitle(FullTitle).ToString());

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestGetAllGameDataIDs_TooltipText,
                                 "UnrealPokemon.PokemonData.Nodes.TestGetAllGameDataIDs.TooltipText",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestGetAllGameDataIDs_TooltipText::RunTest(const FString &Parameters) {
    FBlueprintActionDatabase::Get().RefreshAll();

    MakeTestableBP(TestBP, TestGraph);

    auto TestNode = NewObject<UK2Node_GetAllGameDataIDs>(TestGraph.Get());
    ASSERT_EQUAL(TEXT("Invalid Struct Type"), TestNode->GetTooltipText().ToString());
    TestNode->Initialize(FStat::StaticStruct());
    TestGraph->AddNode(TestNode);

    ASSERT_EQUAL(TEXT("Get All Stat IDs \n\nRepresents one of the stats in the database."), TestNode->GetTooltipText().ToString());

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestGetAllGameDataIDs_MenuCategory,
                                 "UnrealPokemon.PokemonData.Nodes.TestGetAllGameDataIDs.MenuCategory",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestGetAllGameDataIDs_MenuCategory::RunTest(const FString &Parameters) {
    FBlueprintActionDatabase::Get().RefreshAll();

    MakeTestableBP(TestBP, TestGraph);

    auto TestNode = NewObject<UK2Node_GetAllGameDataIDs>(TestGraph.Get());
    ASSERT_EQUAL(FText::GetEmpty().ToString(), TestNode->GetMenuCategory().ToString());
    TestNode->Initialize(FSpeciesData::StaticStruct());
    TestGraph->AddNode(TestNode);

    ASSERT_EQUAL(TEXT("Data|PBS|Species Data"), TestNode->GetMenuCategory().ToString());

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestGetAllGameDataIDs_IconAndTint,
                                 "UnrealPokemon.PokemonData.Nodes.TestGetAllGameDataIDs.IconAndTint",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestGetAllGameDataIDs_IconAndTint::RunTest(const FString &Parameters) {
    FBlueprintActionDatabase::Get().RefreshAll();

    MakeTestableBP(TestBP, TestGraph);

    auto TestNode = NewObject<UK2Node_GetAllGameDataIDs>(TestGraph.Get());
    TestNode->Initialize(FItem::StaticStruct());
    TestGraph->AddNode(TestNode);

    FLinearColor Color;
    auto Icon = TestNode->GetIconAndTint(Color);

    ASSERT_EQUAL(GetDefault<UGraphEditorSettings>()->PureFunctionCallNodeTitleColor, Color);
    ASSERT_EQUAL(FSlateIcon(FAppStyle::GetAppStyleSetName(), "Kismet.AllClasses.FunctionIcon"), Icon);

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestGetAllGameDataIDs_MenuActions,
                                 "UnrealPokemon.PokemonData.Nodes.TestGetAllGameDataIDs.MenuActions",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestGetAllGameDataIDs_MenuActions::RunTest(const FString &Parameters) {
    FBlueprintActionDatabase::Get().RefreshAll();
    auto Actions = FBlueprintActionDatabase::Get().GetAllActions();
    ASSERT_TRUE(Actions.Contains(UK2Node_GetAllGameDataIDs::StaticClass()));
    auto NodeActions = Actions[UK2Node_GetAllGameDataIDs::StaticClass()];
    ASSERT_EQUAL(FDataManager::GetInstance().GetStructTypes().Num(), NodeActions.Num());

    return true;
}

#endif