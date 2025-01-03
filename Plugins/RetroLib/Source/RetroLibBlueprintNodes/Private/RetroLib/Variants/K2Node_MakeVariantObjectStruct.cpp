// "Unreal Pokémon" created by Retro & Chill.

#include "RetroLib/Variants/K2Node_MakeVariantObjectStruct.h"
#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintNodeSpawner.h"
#include "K2Node_CallFunction.h"
#include "KismetCompiler.h"
#include "RetroLib/Blueprints/BlueprintPins.h"
#include "RetroLib/Variants/VariantObjectStruct.h"
#include "RetroLib/Variants/VariantObjectUtilities.h"

void UK2Node_MakeVariantObjectStruct::Initialize(UScriptStruct *Output) {
    OutputType = Output;
}

void UK2Node_MakeVariantObjectStruct::AllocateDefaultPins() {
    CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Wildcard, Retro::PN_Object);
    CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Struct, (OutputType != nullptr ? OutputType.Get() : nullptr),
              UEdGraphSchema_K2::PN_ReturnValue);

    Super::AllocateDefaultPins();
}

void UK2Node_MakeVariantObjectStruct::PostReconstructNode() {
    Super::PostReconstructNode();
    RefreshInputPinType();
}

bool UK2Node_MakeVariantObjectStruct::IsConnectionDisallowed(const UEdGraphPin *MyPin, const UEdGraphPin *OtherPin,
                                                             FString &OutReason) const {
    if (MyPin != GetObjectPin() || MyPin->PinType.PinCategory != UEdGraphSchema_K2::PC_Wildcard) {
        return false;
    }

    auto &Registry = Retro::FVariantObjectStructRegistry::Get();
    auto Registration = Registry.GetVariantStructData(*OutputType);
    check(Registration.IsSet())

    bool bDisallowed = true;
    if (OtherPin->PinType.PinCategory == UEdGraphSchema_K2::PC_Object ||
        OtherPin->PinType.PinCategory == UEdGraphSchema_K2::PC_Interface) {
        auto Class = Cast<UClass>(OtherPin->PinType.PinSubCategoryObject.Get());
        bDisallowed = Class == nullptr || !Registration->IsValidType(Class);
    }

    if (bDisallowed) {
        OutReason = TEXT("Not a valid object type for this variant!");
    }
    return bDisallowed;
}

void UK2Node_MakeVariantObjectStruct::NotifyPinConnectionListChanged(UEdGraphPin *Pin) {
    Super::NotifyPinConnectionListChanged(Pin);
    if (Pin != GetObjectPin()) {
        return;
    }

    RefreshInputPinType();
}

FText UK2Node_MakeVariantObjectStruct::GetNodeTitle(ENodeTitleType::Type TitleType) const {
    auto StructName =
        OutputType != nullptr ? OutputType->GetDisplayNameText() : FText::FromStringView(TEXT("<<INVALID>>"));
    return FText::FormatNamed(NSLOCTEXT("K2Node", "MakeVariantObjectStruct_GetNodeTitle", "Make {Output}"),
                              TEXT("Output"), StructName);
}

FText UK2Node_MakeVariantObjectStruct::GetTooltipText() const {
    auto StructName =
        OutputType != nullptr ? OutputType->GetDisplayNameText() : FText::FromStringView(TEXT("<<INVALID>>"));
    return FText::FormatNamed(NSLOCTEXT("K2Node", "MakeVariantObjectStruct_GetTooltipText",
                                        "Create a new {Output} structure from the source object."),
                              TEXT("Output"), StructName);
}

void UK2Node_MakeVariantObjectStruct::EarlyValidation(FCompilerResultsLog &MessageLog) const {
    Super::EarlyValidation(MessageLog);

    if (auto InputClass = GetInputClass(); !InputClass.IsSet()) {
        MessageLog.Error(TEXT("Must have a valid connection to the input pin"));
    }
}

void UK2Node_MakeVariantObjectStruct::ExpandNode(FKismetCompilerContext &CompilerContext, UEdGraph *SourceGraph) {
    Super::ExpandNode(CompilerContext, SourceGraph);

    // FUNCTION NODE
    const FName FunctionName = GET_FUNCTION_NAME_CHECKED(UVariantObjectUtilities, CreateVariantFromObject);
    auto CallCreateVariant = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
    CallCreateVariant->FunctionReference.SetExternalMember(FunctionName, UVariantObjectUtilities::StaticClass());
    CallCreateVariant->AllocateDefaultPins();

    static const FName Object_ParamName(TEXT("Object"));
    static const FName Variant_ParamName(TEXT("Variant"));

    auto InputPin = FindPinChecked(Retro::PN_Object);
    auto ReturnValuePin = FindPinChecked(UEdGraphSchema_K2::PN_ReturnValue);

    auto CallCreateObjectPin = CallCreateVariant->FindPinChecked(Object_ParamName);
    auto CallCreateVariantPin = CallCreateVariant->FindPinChecked(Variant_ParamName);

    CallCreateObjectPin->PinType = InputPin->PinType;
    CallCreateObjectPin->PinType.PinSubCategoryObject = InputPin->PinType.PinSubCategoryObject;
    CompilerContext.MovePinLinksToIntermediate(*InputPin, *CallCreateObjectPin);

    CallCreateVariantPin->PinType = ReturnValuePin->PinType;
    CallCreateVariantPin->PinType.PinSubCategoryObject = ReturnValuePin->PinType.PinSubCategoryObject;
    CompilerContext.MovePinLinksToIntermediate(*ReturnValuePin, *CallCreateVariantPin);

    BreakAllNodeLinks();
}

void UK2Node_MakeVariantObjectStruct::AddMenuOptionsForStruct(FBlueprintActionDatabaseRegistrar &ActionRegistrar,
                                                              Retro::IVariantRegistration &Registration) const {
    using FCustomizeDelegate = UBlueprintNodeSpawner::FCustomizeNodeDelegate;
    auto CustomizeCallback = [](UEdGraphNode *Node, bool, UScriptStruct *Output) {
        auto TypedNode = CastChecked<UK2Node_MakeVariantObjectStruct>(Node);
        TypedNode->Initialize(Output);
    };

    auto ActionKey = GetClass();
    auto Struct = Registration.GetStructType();
    auto Spawner = UBlueprintNodeSpawner::Create(ActionKey);
    check(Spawner != nullptr)
    Spawner->CustomizeNodeDelegate = FCustomizeDelegate::CreateStatic(CustomizeCallback, Struct);
    ActionRegistrar.AddBlueprintAction(ActionKey, Spawner);
}

UEdGraphPin *UK2Node_MakeVariantObjectStruct::GetObjectPin() const {
    return FindPin(Retro::PN_Object);
}

TOptional<UClass &> UK2Node_MakeVariantObjectStruct::GetInputClass() const {
    auto ObjectPin = GetObjectPin();
    if (ObjectPin == nullptr || ObjectPin->PinType.PinCategory == UEdGraphSchema_K2::PC_Wildcard) {
        return nullptr;
    }

    return Cast<UClass>(ObjectPin->PinType.PinSubCategoryObject.Get());
}

void UK2Node_MakeVariantObjectStruct::RefreshInputPinType() const {
    if (auto ObjectPin = GetObjectPin(); ObjectPin->LinkedTo.Num() > 0) {
        check(ObjectPin->LinkedTo.Num() == 1)
        auto Pin = ObjectPin->LinkedTo[0];
        ObjectPin->PinType = Pin->PinType;
        ObjectPin->PinType.PinSubCategoryObject = Pin->PinType.PinSubCategoryObject;
    } else {
        ObjectPin->PinType.PinCategory = UEdGraphSchema_K2::PC_Wildcard;
        ObjectPin->PinType.PinSubCategoryObject = nullptr;
    }
}