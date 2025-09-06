#include "Asserts.h"
#include "Bag/Item.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "Misc/AutomationTest.h"
#include "Pins/DataHandlePinFactory.h"
#include "SGraphPin.h"
#include "SSearchableComboBox.h"
#include "Utilities/K2Nodes.h"
#include "Utilities/WidgetTestUtilities.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestHandlePinFactory, "Unit Tests.Data.TestHandlePinFactory",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestHandlePinFactory::RunTest(const FString &Parameters)
{
    MakeTestableBP(TestBP, TestGraph);

    FDataHandlePinFactory PinFactory;
    auto Node = NewObject<UEdGraphNode>(TestGraph.Get());

    auto Pin1 = Node->CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Name, TEXT("Input1"));
    auto PinWidget1 = PinFactory.CreatePin(Pin1);
    UE_ASSERT_NULL(PinWidget1.Get());

    auto Pin2 = Node->CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Struct, FItem::StaticStruct(), TEXT("Input2"));
    auto PinWidget2 = PinFactory.CreatePin(Pin2);
    UE_ASSERT_NULL(PinWidget2.Get());

    auto Pin3 = Node->CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Struct, FItemHandle::StaticStruct(), TEXT("Input3"));
    auto PinWidget3 = PinFactory.CreatePin(Pin3);
    UE_CHECK_EQUAL(TEXT("SDataHandlePinStructPin"), PinWidget3->GetType().ToString());

    auto Pin3Combo = FIND_FIRST_CHILD_WIDGET_OF_TYPE(*PinWidget3, SSearchableComboBox);
    UE_ASSERT_NOT_NULL(Pin3Combo.Get());
    Pin3Combo->SetSelectedItem(MakeShared<FString>("POTION"));
    UE_ASSERT_EQUAL(TEXT("POTION"), *Pin3Combo->GetSelectedItem());

    auto Pin4 = Node->CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Struct, FPocketKey::StaticStruct(), TEXT("Input4"));
    auto PinWidget4 = PinFactory.CreatePin(Pin4);
    UE_CHECK_EQUAL(TEXT("SPocketKeyPin"), PinWidget4->GetType().ToString());

    auto Pin4Combo = FIND_FIRST_CHILD_WIDGET_OF_TYPE(*PinWidget4, SSearchableComboBox);
    UE_ASSERT_NOT_NULL(Pin4Combo.Get());
    Pin4Combo->SetSelectedItem(MakeShared<FString>("Medicine"));
    UE_ASSERT_EQUAL(TEXT("Medicine"), *Pin4Combo->GetSelectedItem());

    return true;
}