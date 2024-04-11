// "Unreal Pokémon" created by Retro & Chill.
#include "Bag/Item.h"
#include "BlueprintActionDatabase.h"
#include "BlueprintTypePromotion.h"
#include "CommonDefines.h"
#include "DataManager.h"
#include "Exp/GrowthRateData.h"
#include "GraphEditorSettings.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "Misc/AutomationTest.h"
#include "Nodes/K2Node_GetGameData.h"
#include "Species/SpeciesData.h"
#include "Species/Stat.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestGetGameData_NodeTile, "UnrealPokemon.PokemonData.Nodes.TestGetGameData.NodeTitle",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestGetGameData_NodeTile::RunTest(const FString &Parameters) {
    using enum ENodeTitleType::Type;
    FBlueprintActionDatabase::Get().RefreshAll();

    MakeTestableBP(TestBP, TestGraph);

    auto TestNode = NewObject<UK2Node_GetGameData>(TestGraph.Get());
    AUTOMATION_ASSERT(TestEqual(TEXT("Node title should be class name!"), TestNode->GetNodeTitle(MenuTitle).ToString(),
                                TEXT("Invalid Struct Type")));
    TestNode->Initialize(FGrowthRateData::StaticStruct());
    TestGraph->AddNode(TestNode);

    AUTOMATION_ASSERT(TestEqual(TEXT("Node title should contain superclass!"),
                                TestNode->GetNodeTitle(MenuTitle).ToString(), TEXT("Get Growth Rate Data")));
    AUTOMATION_ASSERT(TestEqual(TEXT("Node title should be superclass!"), TestNode->GetNodeTitle(FullTitle).ToString(),
                                TEXT("Growth Rate Data")));
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestGetGameData_TooltipText,
                                 "UnrealPokemon.PokemonData.Nodes.TestGetGameData.TooltipText",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestGetGameData_TooltipText::RunTest(const FString &Parameters) {
    FBlueprintActionDatabase::Get().RefreshAll();

    MakeTestableBP(TestBP, TestGraph);

    auto TestNode = NewObject<UK2Node_GetGameData>(TestGraph.Get());
    AUTOMATION_ASSERT(
        TestEqual(TEXT("Invalid tooltip text!"), TestNode->GetTooltipText().ToString(), TEXT("Invalid Struct Type")));
    TestNode->Initialize(FStat::StaticStruct());
    TestGraph->AddNode(TestNode);

    AUTOMATION_ASSERT(TestEqual(TEXT("Node title should match struct type!"), TestNode->GetTooltipText().ToString(),
                                TEXT("Get Stat \n\nRepresents one of the stats in the database.")));

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestGetGameData_MenuCategory,
                                 "UnrealPokemon.PokemonData.Nodes.TestGetGameData.MenuCategory",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestGetGameData_MenuCategory::RunTest(const FString &Parameters) {
    FBlueprintActionDatabase::Get().RefreshAll();

    MakeTestableBP(TestBP, TestGraph);

    auto TestNode = NewObject<UK2Node_GetGameData>(TestGraph.Get());
    AUTOMATION_ASSERT(TestEqual(TEXT("Should be original category!"), TestNode->GetMenuCategory().ToString(),
                                FText::GetEmpty().ToString()));
    TestNode->Initialize(FSpeciesData::StaticStruct());
    TestGraph->AddNode(TestNode);

    AUTOMATION_ASSERT(TestEqual(TEXT("Node category should match struct!"), TestNode->GetMenuCategory().ToString(),
                                TEXT("Data|PBS|Species Data")));

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

    AUTOMATION_ASSERT(TestEqual(TEXT("Color should be a pure function!"), Color,
                                GetDefault<UGraphEditorSettings>()->PureFunctionCallNodeTitleColor));
    AUTOMATION_ASSERT(TestEqual(TEXT("Icon should be for a function!"), Icon,
                                FSlateIcon(FAppStyle::GetAppStyleSetName(), "Kismet.AllClasses.FunctionIcon")));

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestGetGameData_MenuActions,
                                 "UnrealPokemon.PokemonData.Nodes.TestGetGameData.MenuActions",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestGetGameData_MenuActions::RunTest(const FString &Parameters) {
    FBlueprintActionDatabase::Get().RefreshAll();
    auto Actions = FBlueprintActionDatabase::Get().GetAllActions();
    AUTOMATION_ASSERT(
        TestTrue(TEXT("Node should have actions!"), Actions.Contains(UK2Node_GetGameData::StaticClass())));
    auto NodeActions = Actions[UK2Node_GetGameData::StaticClass()];
    AUTOMATION_ASSERT(TestEqual(TEXT("Action count should match found database tables!"), NodeActions.Num(),
                                FDataManager::GetInstance().GetStructTypes().Num()));

    return true;
}