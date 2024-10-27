#include "Asserts.h"
#include "Assets/K2Node_LoadAssetByName.h"
#include "Assets/K2Node_LookupAssetByName.h"
#include "Audio/AssetClasses.h"
#include "Graphics/AssetClasses.h"
#include "K2Node_CallFunction.h"
#include "Misc/AutomationTest.h"
#include "Utilities/K2Nodes.h"
#include "UtilityClasses/Helpers/ToolMenuActionAccess.h"

BEGIN_DEFINE_SPEC(FTestLoadAndLookupAssets, "Unit Tests.Assets.BattleRender",
                  EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter);

DeclareTestableBP(TestBP, TestGraph);

FORCEINLINE void AssertValidNode(UK2Node *Node) {
    UE_CHECK_FALSE(Node->GetNodeTitle(ENodeTitleType::MenuTitle).IsEmpty());
    UE_CHECK_FALSE(Node->GetTooltipText().IsEmpty());
    UE_CHECK_FALSE(Node->IsNodePure());
}

END_DEFINE_SPEC(FTestLoadAndLookupAssets);

void FTestLoadAndLookupAssets::Define() {
    Describe("Test Asset Loading/Lookup Blueprint Nodes", [this] {
        BeforeEach([this] { DefineTestableBP(TestBP, TestGraph); });

        AfterEach([this] { CleanUpTestableBP(TestBP, TestGraph); });

        It("Verify wildcard pin accepts FName, FString, and FText", [this] {
            auto &AssetClass = Pokemon::Assets::Audio::PokemonCries;
            auto Node = NewObject<UK2Node_LoadAssetByName>(TestGraph.Get());
            Node->Initialize(AssetClass.GetKey());
            TestGraph->AddNode(Node);
            Node->AllocateDefaultPins();
            AssertValidNode(Node);

            auto InputPin = Node->FindPin(FName("AssetName"));
            UE_ASSERT_NOT_NULL(InputPin);

            MakeTestableNode(DummyInput, TestGraph.Get());
            TArray<UEdGraphPin *> Pins;
            MakeTestPin(DummyInput, Pins, NamePin, UEdGraphSchema_K2::PC_Name, EGPD_Output);
            MakeTestPin(DummyInput, Pins, StringPin, UEdGraphSchema_K2::PC_String, EGPD_Output);
            MakeTestPin(DummyInput, Pins, TextPin, UEdGraphSchema_K2::PC_Text, EGPD_Output);
            MakeTestPin(DummyInput, Pins, IntPin, UEdGraphSchema_K2::PC_Int, EGPD_Output);

            FString ErrorMessage;
            UE_ASSERT_FALSE(Node->IsConnectionDisallowed(InputPin, NamePin, ErrorMessage));
            UE_ASSERT_FALSE(Node->IsConnectionDisallowed(InputPin, StringPin, ErrorMessage));
            UE_ASSERT_FALSE(Node->IsConnectionDisallowed(InputPin, TextPin, ErrorMessage));
            UE_ASSERT_TRUE(Node->IsConnectionDisallowed(InputPin, IntPin, ErrorMessage));

            NamePin->MakeLinkTo(InputPin);
            Node->NotifyPinConnectionListChanged(InputPin);
            UE_CHECK_EQUAL(UEdGraphSchema_K2::PC_Name, InputPin->PinType.PinCategory);

            NamePin->BreakLinkTo(InputPin);
            Node->NotifyPinConnectionListChanged(InputPin);
            UE_CHECK_EQUAL(UEdGraphSchema_K2::PC_Wildcard, InputPin->PinType.PinCategory);

            StringPin->MakeLinkTo(InputPin);
            Node->NotifyPinConnectionListChanged(InputPin);
            UE_CHECK_EQUAL(UEdGraphSchema_K2::PC_String, InputPin->PinType.PinCategory);

            StringPin->BreakLinkTo(InputPin);
            Node->NotifyPinConnectionListChanged(InputPin);
            UE_CHECK_EQUAL(UEdGraphSchema_K2::PC_Wildcard, InputPin->PinType.PinCategory);

            TextPin->MakeLinkTo(InputPin);
            Node->NotifyPinConnectionListChanged(InputPin);
            UE_CHECK_EQUAL(UEdGraphSchema_K2::PC_Text, InputPin->PinType.PinCategory);

            TextPin->BreakLinkTo(InputPin);
            Node->NotifyPinConnectionListChanged(InputPin);
            UE_CHECK_EQUAL(UEdGraphSchema_K2::PC_Wildcard, InputPin->PinType.PinCategory);

            return true;
        });

        It("Test toggle to between wildcard and string literal mode", [this] {
            auto &AssetClass = Pokemon::Assets::Audio::PokemonCries;
            auto Node = NewObject<UK2Node_LoadAssetByName>(TestGraph.Get());
            Node->Initialize(AssetClass.GetKey());
            TestGraph->AddNode(Node);
            Node->AllocateDefaultPins();
            AssertValidNode(Node);

            auto InputPin = Node->FindPin(FName("AssetName"));
            UE_ASSERT_NOT_NULL(InputPin);
            UE_CHECK_EQUAL(UEdGraphSchema_K2::PC_Wildcard, InputPin->PinType.PinCategory);

            MakeTestableNode(DummyInput, TestGraph.Get());
            TArray<UEdGraphPin *> Pins;
            MakeTestPin(DummyInput, Pins, StringPin, UEdGraphSchema_K2::PC_String, EGPD_Output);
            MakeTestPin(DummyInput, Pins, TextPin, UEdGraphSchema_K2::PC_Text, EGPD_Output);

            TextPin->MakeLinkTo(InputPin);
            Node->NotifyPinConnectionListChanged(InputPin);

            UGraphNodeContextMenuContext *ContextObject = NewObject<UGraphNodeContextMenuContext>();
            ContextObject->Init(TestGraph.Get(), Node, nullptr, false);
            FToolMenuContext Context(ContextObject);
            auto GeneratedMenu = UToolMenus::Get()->GenerateMenu("TestMenu", Context);

            Node->GetNodeContextMenuActions(GeneratedMenu, ContextObject);
            auto Section = GeneratedMenu->FindSection("UK2Node_DynamicAssetLoadBase");
            UE_ASSERT_NOT_NULL(Section);
            auto Option = Section->FindEntry("ToggleWildcard");
            UE_ASSERT_NOT_NULL(Option);
            FPopulateMenuBuilderWithToolMenuEntry::ExecuteOption(*Option);

            InputPin = Node->FindPin(FName("AssetName"));
            UE_CHECK_EQUAL(UEdGraphSchema_K2::PC_String, InputPin->PinType.PinCategory);
            UE_ASSERT_EQUAL(0, InputPin->LinkedTo.Num());

            GeneratedMenu = UToolMenus::Get()->GenerateMenu("TestMenu", Context);
            Node->GetNodeContextMenuActions(GeneratedMenu, ContextObject);
            Section = GeneratedMenu->FindSection("UK2Node_DynamicAssetLoadBase");
            UE_ASSERT_NOT_NULL(Section);
            Option = Section->FindEntry("ToggleWildcard");
            UE_ASSERT_NOT_NULL(Option);
            FPopulateMenuBuilderWithToolMenuEntry::ExecuteOption(*Option);

            InputPin = Node->FindPin(FName("AssetName"));
            UE_CHECK_EQUAL(UEdGraphSchema_K2::PC_Wildcard, InputPin->PinType.PinCategory);

            StringPin->MakeLinkTo(InputPin);
            Node->NotifyPinConnectionListChanged(InputPin);
            UE_CHECK_EQUAL(UEdGraphSchema_K2::PC_String, InputPin->PinType.PinCategory);
            UE_ASSERT_EQUAL(1, InputPin->LinkedTo.Num());
            GeneratedMenu = UToolMenus::Get()->GenerateMenu("TestMenu", Context);
            Node->GetNodeContextMenuActions(GeneratedMenu, ContextObject);
            Section = GeneratedMenu->FindSection("UK2Node_DynamicAssetLoadBase");
            UE_ASSERT_NOT_NULL(Section);
            Option = Section->FindEntry("ToggleWildcard");
            UE_ASSERT_NOT_NULL(Option);
            FPopulateMenuBuilderWithToolMenuEntry::ExecuteOption(*Option);

            InputPin = Node->FindPin(FName("AssetName"));
            UE_CHECK_EQUAL(UEdGraphSchema_K2::PC_String, InputPin->PinType.PinCategory);
            UE_ASSERT_EQUAL(1, InputPin->LinkedTo.Num());
            return true;
        });

        It("Test load UObject asset class", [this] {
            auto &AssetClass = Pokemon::Assets::Audio::PokemonCries;
            auto Node = NewObject<UK2Node_LoadAssetByName>(TestGraph.Get());
            Node->Initialize(AssetClass.GetKey());
            TestGraph->AddNode(Node);
            Node->AllocateDefaultPins();
            AssertValidNode(Node);

            auto OutputPin = Node->FindPin(UEdGraphSchema_K2::PN_ReturnValue);
            UE_ASSERT_NOT_NULL(OutputPin);

            UE_ASSERT_EQUAL(UEdGraphSchema_K2::PC_Object, OutputPin->PinType.PinCategory);
            UE_ASSERT_TRUE(OutputPin->PinType.PinSubCategoryObject == USoundBase::StaticClass());

            return true;
        });

        It("Test load variant asset class", [this] {
            auto &AssetClass = Pokemon::Assets::Graphics::SummaryBalls;
            auto Node = NewObject<UK2Node_LoadAssetByName>(TestGraph.Get());
            Node->Initialize(AssetClass.GetKey());
            TestGraph->AddNode(Node);
            Node->AllocateDefaultPins();
            AssertValidNode(Node);

            auto OutputPin = Node->FindPin(UEdGraphSchema_K2::PN_ReturnValue);
            UE_ASSERT_NOT_NULL(OutputPin);

            UE_ASSERT_EQUAL(UEdGraphSchema_K2::PC_Struct, OutputPin->PinType.PinCategory);
            UE_ASSERT_TRUE(OutputPin->PinType.PinSubCategoryObject == UE::Ranges::GetScriptStruct<FImageAsset>());

            return true;
        });

        It("Test lookup UObject asset class", [this] {
            auto &AssetClass = Pokemon::Assets::Audio::PokemonCries;
            auto Node = NewObject<UK2Node_LookupAssetByName>(TestGraph.Get());
            Node->Initialize(AssetClass.GetKey());
            TestGraph->AddNode(Node);
            Node->AllocateDefaultPins();
            AssertValidNode(Node);

            auto OutputPin = Node->FindPin(UEdGraphSchema_K2::PN_ReturnValue);
            UE_ASSERT_NOT_NULL(OutputPin);

            UE_ASSERT_EQUAL(UEdGraphSchema_K2::PC_SoftObject, OutputPin->PinType.PinCategory);
            UE_ASSERT_TRUE(OutputPin->PinType.PinSubCategoryObject == USoundBase::StaticClass());

            return true;
        });

        It("Test lookup variant asset class", [this] {
            auto &AssetClass = Pokemon::Assets::Graphics::SummaryBalls;
            auto Node = NewObject<UK2Node_LookupAssetByName>(TestGraph.Get());
            Node->Initialize(AssetClass.GetKey());
            TestGraph->AddNode(Node);
            Node->AllocateDefaultPins();
            AssertValidNode(Node);

            auto OutputPin = Node->FindPin(UEdGraphSchema_K2::PN_ReturnValue);
            UE_ASSERT_NOT_NULL(OutputPin);

            UE_ASSERT_EQUAL(UEdGraphSchema_K2::PC_Struct, OutputPin->PinType.PinCategory);
            UE_ASSERT_TRUE(OutputPin->PinType.PinSubCategoryObject == UE::Ranges::GetScriptStruct<FSoftImageAsset>());

            return true;
        });
    });
}