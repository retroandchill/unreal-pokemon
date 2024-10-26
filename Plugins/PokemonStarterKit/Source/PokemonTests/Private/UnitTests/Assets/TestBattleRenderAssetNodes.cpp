#include "Asserts.h"
#include "K2Node_CallFunction.h"
#include "Graphics/BattleRender.h"
#include "Misc/AutomationTest.h"
#include "Ranges/Blueprints/BlueprintPins.h"
#include "Ranges/Variants/K2Node_GetVariantObject.h"
#include "Ranges/Variants/K2Node_GetVariantValue.h"
#include "Ranges/Variants/K2Node_LoadVariantSynchronous.h"
#include "Ranges/Variants/K2Node_MakeSoftVariantFromSoftObject.h"
#include "Ranges/Variants/K2Node_MakeVariantObjectStruct.h"
#include "Utilities/K2Nodes.h"

BEGIN_DEFINE_SPEC(FTestBattleRenderAssetNodes, "Unit Tests.Assets.BattleRender",
                  EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter);

DeclareTestableBP(TestBP, TestGraph);

TObjectPtr<UScriptStruct> StructType;
TObjectPtr<UScriptStruct> SoftStructType;
TOptional<UE::Ranges::IVariantRegistration&> Registration;

template <bool bIsCompact = false>
FORCEINLINE void AssertValidNode(UK2Node* Node) {
    UE_CHECK_FALSE(Node->GetNodeTitle(ENodeTitleType::MenuTitle).IsEmpty());
    UE_CHECK_FALSE(Node->GetTooltipText().IsEmpty());
    if constexpr (bIsCompact) {
        UE_CHECK_TRUE(Node->ShouldDrawCompact());
        UE_CHECK_FALSE(Node->GetCompactNodeTitle().IsEmpty());
    } else {
        UE_CHECK_FALSE(Node->ShouldDrawCompact());
    }
}

END_DEFINE_SPEC(FTestBattleRenderAssetNodes);

class FPopulateMenuBuilderWithToolMenuEntry {

    public:
    static void ExecuteOption(const FToolMenuEntry& Entry) {
        Entry.Action.GetUIAction()->Execute();
    }
    
};

void FTestBattleRenderAssetNodes::Define() {
    Describe("Test Battle Render Blueprint Nodes", [this] {
        BeforeEach([this] {
            auto &Registry = UE::Ranges::FVariantObjectStructRegistry::Get();
            StructType = UE::Ranges::GetScriptStruct<FBattleRender>();
            SoftStructType = UE::Ranges::GetScriptStruct<FSoftBattleRender>();
            Registration = Registry.GetVariantStructData(*StructType);
            DefineTestableBP(TestBP, TestGraph); 
        });
        
        AfterEach([this] {
            CleanUpTestableBP(TestBP, TestGraph);
            Registration.Reset();
        });

        It("Test get object from Battle Render", [this] {
            auto Node = NewObject<UK2Node_GetVariantObject>(TestGraph.Get());
            TestGraph->AddNode(Node);
            Node->AllocateDefaultPins();
            AssertValidNode<true>(Node);

            auto InputPin = Node->FindPin(UE::Ranges::PN_Variant);
            UE_ASSERT_NOT_NULL(InputPin);
            auto OutputPin = Node->FindPin(UEdGraphSchema_K2::PN_ReturnValue);
            UE_ASSERT_NOT_NULL(OutputPin);
            
            MakeTestableNode(DummyInput, TestGraph.Get());
            TArray<UEdGraphPin*> Pins;
            MakeTestPin(DummyInput, Pins, ValidPin, UEdGraphSchema_K2::PC_Struct, EGPD_Output);
            ValidPin->PinType.PinSubCategoryObject = StructType;

            FString Message;
            UE_CHECK_FALSE(Node->IsConnectionDisallowed(InputPin, ValidPin, Message));

            MakeTestPin(DummyInput, Pins, InvalidPin, UEdGraphSchema_K2::PC_Object, EGPD_Output);
            InvalidPin->PinType.PinSubCategoryObject = UObject::StaticClass();
            UE_CHECK_TRUE(Node->IsConnectionDisallowed(InputPin, InvalidPin, Message));
            UE_CHECK_FALSE(Node->IsConnectionDisallowed(OutputPin, InvalidPin, Message));

            AddExpectedMessage(TEXT("Must have a valid connection to the input pin"));
            FCompilerResultsLog MessageLog;
            Node->EarlyValidation(MessageLog);
            UE_CHECK_EQUAL(1, MessageLog.NumErrors);

            ValidPin->MakeLinkTo(InputPin);
            Node->NotifyPinConnectionListChanged(InputPin);
            UE_CHECK_EQUAL(UEdGraphSchema_K2::PC_Struct, InputPin->PinType.PinCategory);
            UE_CHECK_TRUE(InputPin->PinType.PinSubCategoryObject == StructType);

            ValidPin->BreakLinkTo(InputPin);
            Node->NotifyPinConnectionListChanged(InputPin);
            UE_CHECK_EQUAL(UEdGraphSchema_K2::PC_Wildcard, InputPin->PinType.PinCategory);
            UE_CHECK_NULL(InputPin->PinType.PinSubCategoryObject.Get());
            
            return true;
        });
        
        It("Test make Battle Render", [this] {
            auto Node = NewObject<UK2Node_MakeVariantObjectStruct>(TestGraph.Get());
            Node->Initialize(StructType);
            TestGraph->AddNode(Node);
            Node->AllocateDefaultPins();
            AssertValidNode(Node);

            auto InputPin = Node->FindPin(UE::Ranges::PN_Object);
            UE_ASSERT_NOT_NULL(InputPin);
            auto OutputPin = Node->FindPin(UEdGraphSchema_K2::PN_ReturnValue);
            UE_ASSERT_NOT_NULL(OutputPin);
            
            MakeTestableNode(DummyInput, TestGraph.Get());
            TArray<UEdGraphPin*> Pins;

            auto Classes = Registration->GetValidClasses();
            FString Message;
            for (auto Class : Classes) {
                MakeTestPin(DummyInput, Pins, ValidPin, UEdGraphSchema_K2::PC_Object, EGPD_Output);
                ValidPin->PinType.PinSubCategoryObject = Class;
                UE_CHECK_FALSE(Node->IsConnectionDisallowed(InputPin, ValidPin, Message));
            }
            
            
            MakeTestPin(DummyInput, Pins, InvalidPin, UEdGraphSchema_K2::PC_Object, EGPD_Output);
            InvalidPin->PinType.PinSubCategoryObject = UObject::StaticClass();
            UE_CHECK_TRUE(Node->IsConnectionDisallowed(InputPin, InvalidPin, Message));
            UE_CHECK_FALSE(Node->IsConnectionDisallowed(OutputPin, InvalidPin, Message));

            AddExpectedMessage(TEXT("Must have a valid connection to the input pin"));
            FCompilerResultsLog MessageLog;
            Node->EarlyValidation(MessageLog);
            UE_CHECK_EQUAL(1, MessageLog.NumErrors);

            Pins[0]->MakeLinkTo(InputPin);
            Node->NotifyPinConnectionListChanged(InputPin);
            UE_CHECK_EQUAL(UEdGraphSchema_K2::PC_Object, InputPin->PinType.PinCategory);
            UE_CHECK_TRUE(InputPin->PinType.PinSubCategoryObject == Classes[0]);

            Pins[0]->BreakLinkTo(InputPin);
            Node->NotifyPinConnectionListChanged(InputPin);
            UE_CHECK_EQUAL(UEdGraphSchema_K2::PC_Wildcard, InputPin->PinType.PinCategory);
            UE_CHECK_NULL(InputPin->PinType.PinSubCategoryObject.Get());
            
            return true;
        });
        
        It("Test make Soft Battle Render", [this] {
            auto Node = NewObject<UK2Node_MakeSoftVariantFromSoftObject>(TestGraph.Get());
            Node->Initialize(StructType);
            TestGraph->AddNode(Node);
            Node->AllocateDefaultPins();
            AssertValidNode(Node);

            auto InputPin = Node->FindPin(UE::Ranges::PN_Object);
            UE_ASSERT_NOT_NULL(InputPin);
            auto OutputPin = Node->FindPin(UEdGraphSchema_K2::PN_ReturnValue);
            UE_ASSERT_NOT_NULL(OutputPin);
            
            MakeTestableNode(DummyInput, TestGraph.Get());
            TArray<UEdGraphPin*> Pins;

            auto Classes = Registration->GetValidClasses();
            FString Message;
            for (auto Class : Classes) {
                MakeTestPin(DummyInput, Pins, ValidPin, UEdGraphSchema_K2::PC_SoftObject, EGPD_Output);
                ValidPin->PinType.PinSubCategoryObject = Class;
                UE_CHECK_FALSE(Node->IsConnectionDisallowed(InputPin, ValidPin, Message));
            }
            
            MakeTestPin(DummyInput, Pins, InvalidPin, UEdGraphSchema_K2::PC_Object, EGPD_Output);
            InvalidPin->PinType.PinSubCategoryObject = UObject::StaticClass();
            UE_CHECK_TRUE(Node->IsConnectionDisallowed(InputPin, InvalidPin, Message));
            UE_CHECK_FALSE(Node->IsConnectionDisallowed(OutputPin, InvalidPin, Message));

            AddExpectedMessage(TEXT("Must have a valid connection to the input pin"));
            FCompilerResultsLog MessageLog;
            Node->EarlyValidation(MessageLog);
            UE_CHECK_EQUAL(1, MessageLog.NumErrors);

            Pins[0]->MakeLinkTo(InputPin);
            Node->NotifyPinConnectionListChanged(InputPin);
            UE_CHECK_EQUAL(UEdGraphSchema_K2::PC_SoftObject, InputPin->PinType.PinCategory);
            UE_CHECK_TRUE(InputPin->PinType.PinSubCategoryObject == Classes[0]);

            Pins[0]->BreakLinkTo(InputPin);
            Node->NotifyPinConnectionListChanged(InputPin);
            UE_CHECK_EQUAL(UEdGraphSchema_K2::PC_Wildcard, InputPin->PinType.PinCategory);
            UE_CHECK_NULL(InputPin->PinType.PinSubCategoryObject.Get());
            
            return true;
        });
        
        It("Test cast node can toggle visibility", [this] {
            for (auto Classes = Registration->GetValidClasses(); auto Class : Classes) {
                auto Node = NewObject<UK2Node_GetVariantValue>(TestGraph.Get());
                Node->Initialize(StructType, Class);
                TestGraph->AddNode(Node);
                Node->AllocateDefaultPins();
                AssertValidNode(Node);

                UE_ASSERT_FALSE(Node->IsNodePure());

                UGraphNodeContextMenuContext* ContextObject = NewObject<UGraphNodeContextMenuContext>();
                ContextObject->Init(TestGraph.Get(), Node, nullptr, false);
                FToolMenuContext Context(ContextObject);
                auto GeneratedMenu = UToolMenus::Get()->GenerateMenu("TestMenu", Context);

                Node->GetNodeContextMenuActions(GeneratedMenu, ContextObject);
                auto Section = GeneratedMenu->FindSection("UK2Node_VariantCastBase");
                UE_ASSERT_NOT_NULL(Section);
                auto Option = Section->FindEntry("TogglePurity");
                UE_ASSERT_NOT_NULL(Option);
                FPopulateMenuBuilderWithToolMenuEntry::ExecuteOption(*Option);
                
                UE_ASSERT_TRUE(Node->IsNodePure());
                GeneratedMenu = UToolMenus::Get()->GenerateMenu("TestMenu", Context);
                Node->GetNodeContextMenuActions(GeneratedMenu, ContextObject);
                Section = GeneratedMenu->FindSection("UK2Node_VariantCastBase");
                UE_ASSERT_NOT_NULL(Section);
                Option = Section->FindEntry("TogglePurity");
                UE_ASSERT_NOT_NULL(Section);
                FPopulateMenuBuilderWithToolMenuEntry::ExecuteOption(*Option);
                
                UE_ASSERT_FALSE(Node->IsNodePure());
            }
            
            return true;
        });

        It("Test make load battle render synchronously", [this] {
            auto Node = NewObject<UK2Node_LoadVariantSynchronous>(TestGraph.Get());
            TestGraph->AddNode(Node);
            Node->AllocateDefaultPins();
            AssertValidNode(Node);
            UE_CHECK_FALSE(Node->IsNodePure());

            auto InputPin = Node->FindPin(UE::Ranges::PN_SoftReference);
            UE_ASSERT_NOT_NULL(InputPin);
            auto OutputPin = Node->FindPin(UEdGraphSchema_K2::PN_ReturnValue);
            UE_ASSERT_NOT_NULL(OutputPin);
            
            MakeTestableNode(DummyInput, TestGraph.Get());
            TArray<UEdGraphPin*> Pins;
            MakeTestPin(DummyInput, Pins, ValidInputPin, UEdGraphSchema_K2::PC_Struct, EGPD_Output);
            ValidInputPin->PinType.PinSubCategoryObject = SoftStructType;
            MakeTestPin(DummyInput, Pins, ValidOutputPin, UEdGraphSchema_K2::PC_Struct, EGPD_Input);
            ValidOutputPin->PinType.PinSubCategoryObject = StructType;

            FString Message;
            UE_CHECK_FALSE(Node->IsConnectionDisallowed(InputPin, ValidInputPin, Message));
            UE_CHECK_FALSE(Node->IsConnectionDisallowed(OutputPin, ValidOutputPin, Message));

            MakeTestPin(DummyInput, Pins, InvalidInputPin, UEdGraphSchema_K2::PC_Object, EGPD_Output);
            InvalidInputPin->PinType.PinSubCategoryObject = UObject::StaticClass();
            MakeTestPin(DummyInput, Pins, InalidOutputPin, UEdGraphSchema_K2::PC_Object, EGPD_Input);
            InalidOutputPin->PinType.PinSubCategoryObject = UObject::StaticClass();
            UE_CHECK_TRUE(Node->IsConnectionDisallowed(InputPin, InvalidInputPin, Message));
            UE_CHECK_TRUE(Node->IsConnectionDisallowed(OutputPin, InalidOutputPin, Message));

            AddExpectedMessage(TEXT("Must have a valid connection to the input pin"));
            AddExpectedMessage(TEXT("Must have a valid connection to the output pin"));
            FCompilerResultsLog MessageLog;
            Node->EarlyValidation(MessageLog);
            UE_CHECK_EQUAL(2, MessageLog.NumErrors);

            ValidInputPin->MakeLinkTo(InputPin);
            Node->NotifyPinConnectionListChanged(InputPin);
            UE_CHECK_EQUAL(UEdGraphSchema_K2::PC_Struct, InputPin->PinType.PinCategory);
            UE_CHECK_TRUE(InputPin->PinType.PinSubCategoryObject == SoftStructType);
            UE_CHECK_EQUAL(UEdGraphSchema_K2::PC_Struct, OutputPin->PinType.PinCategory);
            UE_CHECK_TRUE(OutputPin->PinType.PinSubCategoryObject == StructType);

            ValidInputPin->BreakLinkTo(InputPin);
            Node->NotifyPinConnectionListChanged(InputPin);
            UE_CHECK_EQUAL(UEdGraphSchema_K2::PC_Wildcard, InputPin->PinType.PinCategory);
            UE_CHECK_NULL(InputPin->PinType.PinSubCategoryObject.Get());
            UE_CHECK_EQUAL(UEdGraphSchema_K2::PC_Wildcard, OutputPin->PinType.PinCategory);
            UE_CHECK_NULL(OutputPin->PinType.PinSubCategoryObject.Get());

            ValidOutputPin->MakeLinkTo(OutputPin);
            Node->NotifyPinConnectionListChanged(OutputPin);
            UE_CHECK_EQUAL(UEdGraphSchema_K2::PC_Struct, InputPin->PinType.PinCategory);
            UE_CHECK_TRUE(InputPin->PinType.PinSubCategoryObject == SoftStructType);
            UE_CHECK_EQUAL(UEdGraphSchema_K2::PC_Struct, OutputPin->PinType.PinCategory);
            UE_CHECK_TRUE(OutputPin->PinType.PinSubCategoryObject == StructType);
            
            ValidOutputPin->BreakLinkTo(OutputPin);
            Node->NotifyPinConnectionListChanged(OutputPin);
            UE_CHECK_EQUAL(UEdGraphSchema_K2::PC_Wildcard, InputPin->PinType.PinCategory);
            UE_CHECK_NULL(InputPin->PinType.PinSubCategoryObject.Get());
            UE_CHECK_EQUAL(UEdGraphSchema_K2::PC_Wildcard, OutputPin->PinType.PinCategory);
            UE_CHECK_NULL(OutputPin->PinType.PinSubCategoryObject.Get());
            
            return true;
        });
    });
}