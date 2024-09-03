#include "Asserts.h"
#include "BlueprintActionDatabase.h"
#include "DataManager.h"
#include "Data/K2Node_IsGameDataIDValid.h"
#include "Data/K2Node_SwitchOnDataHandle.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "Misc/AutomationTest.h"
#include "Ranges/Algorithm/ToArray.h"
#include "Ranges/Algorithm/ToSet.h"
#include "Ranges/Views/Filter.h"
#include "Ranges/Views/ContainerView.h"
#include "Ranges/Views/Map.h"
#include "Utilities/K2Nodes.h"
#include "Species/Ability.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestSwitchOnDataHandle, "Unit Tests.Nodes.TestSwitchOnDataHandle",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestSwitchOnDataHandle::RunTest(const FString &Parameters) {
    using enum ENodeTitleType::Type;
    FBlueprintActionDatabase::Get().RefreshAll();

    MakeTestableBP(TestBP, TestGraph);

    auto TestNode = NewObject<UK2Node_SwitchOnDataHandle>(TestGraph.Get());
    UE_CHECK_EQUAL(TEXT("Switch on <<Invalid>>"), TestNode->GetNodeTitle(MenuTitle).ToString());

    TestNode->Initialize(FAbilityHandle::StaticStruct());
    UE_CHECK_EQUAL(TEXT("Switch on Ability Handle"), TestNode->GetNodeTitle(MenuTitle).ToString());
    UE_CHECK_EQUAL(TEXT("Selects an output that matches the input value"), TestNode->GetTooltipText().ToString());
    UE_CHECK_TRUE(TestNode->ShouldShowNodeProperties());

    UE_CHECK_TRUE(TestNode->GetStructType() == FAbilityHandle::StaticStruct());

    auto PinType = TestNode->GetPinType();
    UE_CHECK_EQUAL(UEdGraphSchema_K2::PC_Struct, PinType.PinCategory);
    UE_CHECK_TRUE(PinType.PinSubCategoryObject == FAbilityHandle::StaticStruct());

    UE_CHECK_EQUAL(UEdGraphSchema_K2::PC_Name, TestNode->GetInnerCaseType().PinCategory);

    for (int32 i = 0; i < 5; i++) {
        TestNode->AddPinToSwitchNode();
    }
    
    auto AllPins = TestNode->GetAllPins();
    auto PinNames = AllPins |
        UE::Ranges::Filter([](const UEdGraphPin* Pin) { return Pin->PinType.PinCategory == UEdGraphSchema_K2::PC_Exec; }) |
        UE::Ranges::Filter([](const UEdGraphPin* Pin) { return Pin->Direction == EGPD_Output; }) |
        UE::Ranges::Map(&UEdGraphPin::PinName) |
        UE::Ranges::Filter([](FName Name) { return Name != FName("Default"); }) |
        UE::Ranges::ToArray;

    UE_ASSERT_EQUAL(5, PinNames.Num());
    auto &AbilityTable = FDataManager::GetInstance().GetDataTable<FAbility>();
    for (auto RowName : PinNames) {
        UE_CHECK_TRUE(AbilityTable.IsRowNameValid(RowName));
    }
    
    return true;
}