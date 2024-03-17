// "Unreal Pokémon" created by Retro & Chill.
#include "BlueprintActionDatabase.h"
#include "BlueprintTypePromotion.h"
#include "CommonDefines.h"
#include "DataManager.h"
#include "GraphEditorSettings.h"
#include "Bag/Item.h"
#include "Exp/GrowthRateData.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "Misc/AutomationTest.h"
#include "Nodes/K2Node_IsGameDataIDValid.h"
#include "Species/SpeciesData.h"
#include "Species/Stat.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestIsGameDataIDValid_NodeTile, "Project.PokemonData.Nodes.TestIsGameDataIDValid.NodeTitle",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestIsGameDataIDValid_NodeTile::RunTest(const FString& Parameters) {
	using enum ENodeTitleType::Type;
	FBlueprintActionDatabase::Get().RefreshAll();

	MakeTestableBP(TestBP, TestGraph);

	auto TestNode = NewObject<UK2Node_IsGameDataIDValid>(TestGraph.Get());
	AUTOMATION_ASSERT(TestEqual(TEXT("Node title should be class name!"), TestNode->GetNodeTitle(MenuTitle).ToString(),
		TEXT("Invalid Struct Type")));
	TestNode->Initialize(FGrowthRateData::StaticStruct());
	TestGraph->AddNode(TestNode);
	
	AUTOMATION_ASSERT(TestEqual(TEXT("Node title should contain superclass!"), TestNode->GetNodeTitle(MenuTitle).ToString(),
		TEXT("Is Growth Rate Data ID Valid?")));
	AUTOMATION_ASSERT(TestEqual(TEXT("Node title should be superclass!"), TestNode->GetNodeTitle(FullTitle).ToString(),
		TEXT("Growth Rate Data")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestIsGameDataIDValid_TooltipText, "Project.PokemonData.Nodes.TestIsGameDataIDValid.TooltipText",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestIsGameDataIDValid_TooltipText::RunTest(const FString& Parameters) {
	FBlueprintActionDatabase::Get().RefreshAll();

	MakeTestableBP(TestBP, TestGraph);

	auto TestNode = NewObject<UK2Node_IsGameDataIDValid>(TestGraph.Get());
	AUTOMATION_ASSERT(TestEqual(TEXT("Invalid tooltip text!"), TestNode->GetTooltipText().ToString(),
		TEXT("Invalid Struct Type")));
	TestNode->Initialize(FStat::StaticStruct());
	TestGraph->AddNode(TestNode);
	
	AUTOMATION_ASSERT(TestEqual(TEXT("Node title should match struct type!"),
		TestNode->GetTooltipText().ToString(),
		TEXT("Is Stat ID Valid? \n\nRepresents one of the stats in the database.")));
	
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestIsGameDataIDValid_MenuCategory, "Project.PokemonData.Nodes.TestIsGameDataIDValid.MenuCategory",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestIsGameDataIDValid_MenuCategory::RunTest(const FString& Parameters) {
	FBlueprintActionDatabase::Get().RefreshAll();

	MakeTestableBP(TestBP, TestGraph);

	auto TestNode = NewObject<UK2Node_IsGameDataIDValid>(TestGraph.Get());
	AUTOMATION_ASSERT(TestEqual(TEXT("Should be original category!"), TestNode->GetMenuCategory().ToString(),
		FText::GetEmpty().ToString()));
	TestNode->Initialize(FSpeciesData::StaticStruct());
	TestGraph->AddNode(TestNode);
	
	AUTOMATION_ASSERT(TestEqual(TEXT("Node category should match struct!"),
		TestNode->GetMenuCategory().ToString(),
		TEXT("Data|PBS|Species Data")));
	
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestIsGameDataIDValid_IconAndTint, "Project.PokemonData.Nodes.TestIsGameDataIDValid.IconAndTint",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestIsGameDataIDValid_IconAndTint::RunTest(const FString& Parameters) {
	FBlueprintActionDatabase::Get().RefreshAll();

	MakeTestableBP(TestBP, TestGraph);

	auto TestNode = NewObject<UK2Node_IsGameDataIDValid>(TestGraph.Get());
	TestNode->Initialize(FItem::StaticStruct());
	TestGraph->AddNode(TestNode);

	FLinearColor Color;
	auto Icon = TestNode->GetIconAndTint(Color);
	
	AUTOMATION_ASSERT(TestEqual(TEXT("Color should be a pure function!"),
		Color, GetDefault<UGraphEditorSettings>()->PureFunctionCallNodeTitleColor));
	AUTOMATION_ASSERT(TestEqual(TEXT("Icon should be for a function!"),
		Icon, FSlateIcon(FAppStyle::GetAppStyleSetName(), "Kismet.AllClasses.FunctionIcon")));
	
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestIsGameDataIDValid_MenuActions, "Project.PokemonData.Nodes.TestIsGameDataIDValid.MenuActions",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestIsGameDataIDValid_MenuActions::RunTest(const FString& Parameters) {
	FBlueprintActionDatabase::Get().RefreshAll();
	auto Actions = FBlueprintActionDatabase::Get().GetAllActions();
	AUTOMATION_ASSERT(TestTrue(TEXT("Node should have actions!"), Actions.Contains(UK2Node_IsGameDataIDValid::StaticClass())));
	auto NodeActions = Actions[UK2Node_IsGameDataIDValid::StaticClass()];
	AUTOMATION_ASSERT(TestEqual(TEXT("Action count should match found database tables!"), NodeActions.Num(),
		FDataManager::GetInstance().GetStructTypes().Num()));
	
	return true;
}