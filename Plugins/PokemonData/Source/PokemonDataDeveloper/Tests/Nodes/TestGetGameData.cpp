// "Unreal Pokémon" created by Retro & Chill.
#if WITH_TESTS && HAS_AUTOMATION_HELPERS

#include "Bag/Item.h"
#include "BlueprintActionDatabase.h"
#include "BlueprintTypePromotion.h"
#include "Asserts.h"
#include "DataManager.h"
#include "Exp/GrowthRateData.h"
#include "GraphEditorSettings.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "Misc/AutomationTest.h"
#include "Nodes/K2Node_GetGameData.h"
#include "Species/SpeciesData.h"
#include "Species/Stat.h"
#include "Utilities/K2Nodes.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestGetGameData_NodeTile, "UnrealPokemon.PokemonData.Nodes.TestGetGameData.NodeTitle",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestGetGameData_NodeTile::RunTest(const FString &Parameters) {
    using enum ENodeTitleType::Type;
    FBlueprintActionDatabase::Get().RefreshAll();

    MakeTestableBP(TestBP, TestGraph);

    auto TestNode = NewObject<UK2Node_GetGameData>(TestGraph.Get());
    ASSERT_EQUAL(TEXT("Invalid Struct Type"), TestNode->GetNodeTitle(MenuTitle).ToString());
    TestNode->Initialize(FGrowthRateData::StaticStruct());
    TestGraph->AddNode(TestNode);

    ASSERT_EQUAL(TEXT("Get Growth Rate Data"), TestNode->GetNodeTitle(MenuTitle).ToString());
    ASSERT_EQUAL(TEXT("Growth Rate Data"), TestNode->GetNodeTitle(FullTitle).ToString());
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestGetGameData_TooltipText,
                                 "UnrealPokemon.PokemonData.Nodes.TestGetGameData.TooltipText",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestGetGameData_TooltipText::RunTest(const FString &Parameters) {
    FBlueprintActionDatabase::Get().RefreshAll();

    MakeTestableBP(TestBP, TestGraph);

    auto TestNode = NewObject<UK2Node_GetGameData>(TestGraph.Get());
    ASSERT_EQUAL(TEXT("Invalid Struct Type"), TestNode->GetTooltipText().ToString());
    TestNode->Initialize(FStat::StaticStruct());
    TestGraph->AddNode(TestNode);

    ASSERT_EQUAL(TEXT("Get Stat \n\nRepresents one of the stats in the database."), TestNode->GetTooltipText().ToString());

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestGetGameData_MenuCategory,
                                 "UnrealPokemon.PokemonData.Nodes.TestGetGameData.MenuCategory",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestGetGameData_MenuCategory::RunTest(const FString &Parameters) {
    FBlueprintActionDatabase::Get().RefreshAll();

    MakeTestableBP(TestBP, TestGraph);

    auto TestNode = NewObject<UK2Node_GetGameData>(TestGraph.Get());
    ASSERT_EQUAL(FText::GetEmpty().ToString(), TestNode->GetMenuCategory().ToString());
    TestNode->Initialize(FSpeciesData::StaticStruct());
    TestGraph->AddNode(TestNode);

    ASSERT_EQUAL(TEXT("Data|PBS|Species Data"), TestNode->GetMenuCategory().ToString());

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestGetGameData_IconAndTint,
                                 "UnrealPokemon.PokemonData.Nodes.TestGetGameData.IconAndTint",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestGetGameData_IconAndTint::RunTest(const FString &Parameters) {
    FBlueprintActionDatabase::Get().RefreshAll();

    MakeTestableBP(TestBP, TestGraph);

    auto TestNode = NewObject<UK2Node_GetGameData>(TestGraph.Get());
    TestNode->Initialize(FItem::StaticStruct());
    TestGraph->AddNode(TestNode);

    FLinearColor Color;
    auto Icon = TestNode->GetIconAndTint(Color);

    ASSERT_EQUAL(GetDefault<UGraphEditorSettings>()->PureFunctionCallNodeTitleColor, Color);
    ASSERT_EQUAL(FSlateIcon(FAppStyle::GetAppStyleSetName(), "Kismet.AllClasses.FunctionIcon"), Icon);

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestGetGameData_MenuActions,
                                 "UnrealPokemon.PokemonData.Nodes.TestGetGameData.MenuActions",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestGetGameData_MenuActions::RunTest(const FString &Parameters) {
    FBlueprintActionDatabase::Get().RefreshAll();
    auto Actions = FBlueprintActionDatabase::Get().GetAllActions();
    ASSERT_TRUE(Actions.Contains(UK2Node_GetGameData::StaticClass()));
    auto NodeActions = Actions[UK2Node_GetGameData::StaticClass()];
    ASSERT_EQUAL(FDataManager::GetInstance().GetStructTypes().Num(), NodeActions.Num());

    return true;
}

#endif