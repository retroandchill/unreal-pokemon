// "Unreal Pokémon" created by Retro & Chill.
#include "Nodes/K2Node_DisplayMessageWithChoices.h"

#include "K2Node_MakeArray.h"
#include "K2Node_Select.h"
#include "K2Node_Switch.h"
#include "K2Node_SwitchInteger.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "KismetCompiler.h"
#include "Nodes/DisplayMessageWithChoices.h"

UK2Node_DisplayMessageWithChoices::UK2Node_DisplayMessageWithChoices(const FObjectInitializer &ObjectInitializer)
    : Super(ObjectInitializer) {}

void UK2Node_DisplayMessageWithChoices::AllocateDefaultPins() {
    Super::AllocateDefaultPins();
    static const FName Choices_ParamName(TEXT("Choices"));
    static const FName OnChoiceSelected_ParamName(TEXT("OnChoiceSelected"));
    static const FName Index_ParamName(TEXT("Index"));
    static const FName Choice_ParamName(TEXT("Choice"));

    FindPinChecked(Choices_ParamName)->bHidden = true;
    FindPinChecked(OnChoiceSelected_ParamName)->bHidden = true;
    FindPinChecked(Index_ParamName)->bHidden = true;
    FindPinChecked(Choice_ParamName)->bHidden = true;

    for (int i = 0; i < ChoiceCount; i++) {
        AddInputAndOutputPin(i);
    }
}

void UK2Node_DisplayMessageWithChoices::GetNodeContextMenuActions(UToolMenu *Menu,
                                                                  UGraphNodeContextMenuContext *Context) const {
    Super::GetNodeContextMenuActions(Menu, Context);

    static const FName DisplayMessageWithChoicesNodeName(TEXT("DisplayMessageWithChoices"));
    static const FText DisplayMessageWithChoicesStr =
        NSLOCTEXT("K2Node", "DisplayMessageWithChoices", "Display Message /w Choices");

    // Add the option to remove a pin via the context menu
    if (CanRemovePin(Context->Pin)) {
        FToolMenuSection &Section = Menu->AddSection(DisplayMessageWithChoicesNodeName, DisplayMessageWithChoicesStr);
        Section.AddMenuEntry(
            "RemovePin", NSLOCTEXT("K2Node", "RemovePin", "Remove pin"),
            NSLOCTEXT("K2Node", "RemovePinTooltip", "Remove this input pin"), FSlateIcon(),
            FUIAction(FExecuteAction::CreateUObject(const_cast<UK2Node_DisplayMessageWithChoices *>(this),
                                                    &UK2Node_DisplayMessageWithChoices::RemoveInputPin,
                                                    const_cast<UEdGraphPin *>(Context->Pin))));
    } else if (CanAddPin()) {
        FToolMenuSection &Section = Menu->AddSection(DisplayMessageWithChoicesNodeName, DisplayMessageWithChoicesStr);
        Section.AddMenuEntry(
            "AddPin", NSLOCTEXT("K2Node", "AddPin", "Add pin"),
            NSLOCTEXT("K2Node", "AddPinTooltip", "Add another input pin"), FSlateIcon(),
            FUIAction(FExecuteAction::CreateUObject(const_cast<UK2Node_DisplayMessageWithChoices *>(this),
                                                    &UK2Node_DisplayMessageWithChoices::AddInputPin)));
    }
}

void UK2Node_DisplayMessageWithChoices::GetMenuActions(FBlueprintActionDatabaseRegistrar &ActionRegistrar) const {
    auto FactoryFunc = UDisplayMessageWithChoices::StaticClass()->FindFunctionByName("DisplayMessageWithChoices");
    check(FactoryFunc != nullptr) SupplyMenuActions(ActionRegistrar, FactoryFunc);
}

void UK2Node_DisplayMessageWithChoices::ExpandNode(FKismetCompilerContext &CompilerContext, UEdGraph *SourceGraph) {
    auto ChoicesArrayNode = CompilerContext.SpawnIntermediateNode<UK2Node_MakeArray>(this, SourceGraph);
    ChoicesArrayNode->AllocateDefaultPins();

    // Since K2Node_BaseAsyncTask requires all the pins match the calling function exactly, we need to temporarily
    // remove any pins that we added, and add them back after that part has been compiled
    TArray<UEdGraphPin *> DynamicPins;

    for (int32 i = 0; i < ChoiceCount; i++) {
        auto ChoicePinName = FName(*(FString("Choice") + FString::FromInt(i)));
        auto ChoicePin = FindPinChecked(ChoicePinName);

        auto IndexPinName = FName(*(FString("[") + FString::FromInt(i) + "]"));
        auto IndexPin = ChoicesArrayNode->FindPin(IndexPinName);

        if (IndexPin == nullptr) {
            ChoicesArrayNode->AddInputPin();
            IndexPin = ChoicesArrayNode->FindPinChecked(IndexPinName);
        }
        IndexPin->PinType.PinCategory = UEdGraphSchema_K2::PC_Text;

        CompilerContext.MovePinLinksToIntermediate(*ChoicePin, *IndexPin);

        DynamicPins.Add(ChoicePin);
        Pins.Remove(ChoicePin);
    }

    auto ArrayPinOutput = ChoicesArrayNode->FindPinChecked(TEXT("Array"));
    ArrayPinOutput->PinType.PinCategory = UEdGraphSchema_K2::PC_Text;
    auto ChoicesInputPin = FindPinChecked(TEXT("Choices"));
    ArrayPinOutput->MakeLinkTo(ChoicesInputPin);

    auto ChoiceSwitchNode = CompilerContext.SpawnIntermediateNode<UK2Node_SwitchInteger>(this, SourceGraph);
    ChoiceSwitchNode->AllocateDefaultPins();

    auto SwitchInputPin = ChoiceSwitchNode->FindPinChecked(UEdGraphSchema_K2::PN_Execute);
    auto SwitchOnPin = ChoiceSwitchNode->FindPinChecked(TEXT("Selection"));
    auto OnChoiceSelectedPin = FindPinChecked(TEXT("OnChoiceSelected"));
    auto ChoiceSelectedPin = FindPinChecked(TEXT("Index"));

    OnChoiceSelectedPin->MakeLinkTo(SwitchInputPin);
    ChoiceSelectedPin->MakeLinkTo(SwitchOnPin);

    for (int32 i = 0; i < ChoiceCount; i++) {
        auto ChoiceOutputPinName = FName(*(FString("Choice") + FString::FromInt(i) + "_Output"));
        auto ChoiceOutputPin = FindPinChecked(ChoiceOutputPinName);

        auto CasePinName = ChoiceSwitchNode->GetPinNameGivenIndex(i);
        auto CasePin = ChoiceSwitchNode->FindPin(CasePinName);

        if (CasePin == nullptr) {
            ChoiceSwitchNode->AddPinToSwitchNode();
            CasePin = ChoiceSwitchNode->FindPinChecked(CasePinName);
        }

        CompilerContext.MovePinLinksToIntermediate(*ChoiceOutputPin, *CasePin);
        ReconnectOutputPin(CompilerContext, CasePin);

        DynamicPins.Add(ChoiceOutputPin);
        Pins.Remove(ChoiceOutputPin);
    }

    Super::ExpandNode(CompilerContext, SourceGraph);

    for (auto Pin : DynamicPins) {
        Pins.Add(Pin);
    }

    BreakAllNodeLinks();
}

void UK2Node_DisplayMessageWithChoices::AddInputPin() {
    AddInputAndOutputPin(ChoiceCount);
    ChoiceCount++;
}

void UK2Node_DisplayMessageWithChoices::RemoveInputPin(UEdGraphPin *Pin) {
    if (!CanRemovePin(Pin))
        return;

    Modify();
    if (FRegexMatcher Matcher(FRegexPattern("Choice(\\d+)"), Pin->GetFName().ToString()); Matcher.FindNext()) {
        RemovePin(FindPinChecked(FName(*(FString("Choice" + Matcher.GetCaptureGroup(1) + "_Output")))));
    }
    RemovePin(Pin);
    ChoiceCount--;
    FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(GetBlueprint());
}

bool UK2Node_DisplayMessageWithChoices::CanRemovePin(const UEdGraphPin *Pin) const {
    if (Pin == nullptr)
        return false;

    auto PinName = Pin->GetFName().ToString();
    return PinName.Contains("Choice");
}

void UK2Node_DisplayMessageWithChoices::AddInputAndOutputPin(int Index) {
    FString PinName = FString::Format(TEXT("Choice{Index}"), FStringFormatNamedArguments({{TEXT("Index"), Index}}));
    CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Text, FName(*PinName));

    PinName += "_Output";
    CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, FName(*PinName));
}
