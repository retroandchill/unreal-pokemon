// "Unreal Pokémon" created by Retro & Chill.
#if WITH_TESTS && HAS_AUTOMATION_HELPERS

#include "Bag/Item.h"
#include "BlueprintActionDatabase.h"
#include "DataManager.h"
#include "Exp/GrowthRateData.h"
#include "GraphEditorSettings.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "Misc/AutomationTest.h"
#include "Nodes/K2Node_IsGameDataIDValid.h"
#include "Species/SpeciesData.h"
#include "Species/Stat.h"
#include "Utilities/K2Nodes.h"
#include "Asserts.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestIsGameDataIDValid_NodeTile,
                                 "UnrealPokemon.PokemonData.Nodes.TestIsGameDataIDValid.NodeTitle",
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
                                 "UnrealPokemon.PokemonData.Nodes.TestIsGameDataIDValid.TooltipText",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestIsGameDataIDValid_TooltipText::RunTest(const FString &Parameters) {
    FBlueprintActionDatabase::Get().RefreshAll();

    MakeTestableBP(TestBP, TestGraph);

    auto TestNode = NewObject<UK2Node_IsGameDataIDValid>(TestGraph.Get());
    ASSERT_EQUAL(TEXT("Invalid Struct Type"), TestNode->GetTooltipText().ToString());
    TestNode->Initialize(FStat::StaticStruct());
    TestGraph->AddNode(TestNode);

    ASSERT_EQUAL(TEXT("Is Stat ID Valid? \n\nRepresents one of the stats in the database."), TestNode->GetTooltipText().ToString());

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestIsGameDataIDValid_MenuCategory,
                                 "UnrealPokemon.PokemonData.Nodes.TestIsGameDataIDValid.MenuCategory",
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
                                 "UnrealPokemon.PokemonData.Nodes.TestIsGameDataIDValid.IconAndTint",
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
                                 "UnrealPokemon.PokemonData.Nodes.TestIsGameDataIDValid.MenuActions",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestIsGameDataIDValid_MenuActions::RunTest(const FString &Parameters) {
    FBlueprintActionDatabase::Get().RefreshAll();
    auto Actions = FBlueprintActionDatabase::Get().GetAllActions();
    ASSERT_TRUE(Actions.Contains(UK2Node_IsGameDataIDValid::StaticClass()));
    auto NodeActions = Actions[UK2Node_IsGameDataIDValid::StaticClass()];
    ASSERT_EQUAL(FDataManager::GetInstance().GetStructTypes().Num(), NodeActions.Num());

    return true;
}

#endif