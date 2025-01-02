// "Unreal Pokémon" created by Retro & Chill.

#include "RetroLib/Variants/K2Node_MakeSoftVariantFromSoftObject.h"

#include <RetroLib/Optionals/To.h>

#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintNodeSpawner.h"
#include "K2Node_CallFunction.h"
#include "KismetCompiler.h"
#include "RetroLib/Blueprints/BlueprintPins.h"
#include "RetroLib/Casting/DynamicCast.h"
#include "RetroLib/Optionals/AndThen.h"
#include "RetroLib/Optionals/Transform.h"
#include "RetroLib/Variants/VariantObjectStruct.h"
#include "RetroLib/Variants/VariantObjectUtilities.h"

void UK2Node_MakeSoftVariantFromSoftObject::Initialize(UScriptStruct *SoftReference) {
    SoftReferenceType = SoftReference;
}

void UK2Node_MakeSoftVariantFromSoftObject::AllocateDefaultPins() {
    CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Wildcard, Retro::PN_Object);
    CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Struct,
              (SoftReferenceType != nullptr ? SoftReferenceType.Get() : nullptr), UEdGraphSchema_K2::PN_ReturnValue);

    Super::AllocateDefaultPins();
}

void UK2Node_MakeSoftVariantFromSoftObject::PostReconstructNode() {
    Super::PostReconstructNode();
    RefreshInputPinType();
}

bool UK2Node_MakeSoftVariantFromSoftObject::IsConnectionDisallowed(const UEdGraphPin *MyPin,
                                                                   const UEdGraphPin *OtherPin,
                                                                   FString &OutReason) const {
    if (MyPin != GetObjectPin() || MyPin->PinType.PinCategory != UEdGraphSchema_K2::PC_Wildcard) {
        return false;
    }

    auto &Registry = Retro::FVariantObjectStructRegistry::Get();
    auto Registration = Registry.GetVariantStructData(*SoftReferenceType);
    check(Registration.IsSet())

    bool bDisallowed = true;
    if (OtherPin->PinType.PinCategory == UEdGraphSchema_K2::PC_SoftObject) {
        auto Class = Cast<UClass>(OtherPin->PinType.PinSubCategoryObject.Get());
        bDisallowed = Class == nullptr || !Registration->IsValidType(Class);
    }

    if (bDisallowed) {
        OutReason = TEXT("Not a valid object type for this variant!");
    }
    return bDisallowed;
}

void UK2Node_MakeSoftVariantFromSoftObject::NotifyPinConnectionListChanged(UEdGraphPin *Pin) {
    Super::NotifyPinConnectionListChanged(Pin);
    if (Pin != GetObjectPin()) {
        return;
    }

    RefreshInputPinType();
}

FText UK2Node_MakeSoftVariantFromSoftObject::GetNodeTitle(ENodeTitleType::Type TitleType) const {
    auto StructName = SoftReferenceType != nullptr ? SoftReferenceType->GetDisplayNameText()
                                                   : FText::FromStringView(TEXT("<<INVALID>>"));
    return FText::FormatNamed(NSLOCTEXT("K2Node", "MakeSoftVariantFromSoftObject_GetNodeTitle", "Make {Output}"),
                              TEXT("Output"), StructName);
}

FText UK2Node_MakeSoftVariantFromSoftObject::GetTooltipText() const {
    auto StructName = SoftReferenceType != nullptr ? SoftReferenceType->GetDisplayNameText()
                                                   : FText::FromStringView(TEXT("<<INVALID>>"));
    return FText::FormatNamed(NSLOCTEXT("K2Node", "MakeSoftVariantFromSoftObject_GetNodeTitle",
                                        "Make a new {Output} from the specified soft object"),
                              TEXT("Output"), StructName);
}

void UK2Node_MakeSoftVariantFromSoftObject::EarlyValidation(FCompilerResultsLog &MessageLog) const {

    if (auto InputClass = GetInputClass(); !InputClass.IsSet()) {
        MessageLog.Error(TEXT("Must have a valid connection to the input pin"));
    }
}

void UK2Node_MakeSoftVariantFromSoftObject::ExpandNode(FKismetCompilerContext &CompilerContext, UEdGraph *SourceGraph) {
    Super::ExpandNode(CompilerContext, SourceGraph);

    // FUNCTION NODE
    const FName FunctionName = GET_FUNCTION_NAME_CHECKED(UVariantObjectUtilities, MakeSoftVariantFromSoftObject);
    auto CallCreateVariant = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
    CallCreateVariant->FunctionReference.SetExternalMember(FunctionName, UVariantObjectUtilities::StaticClass());
    CallCreateVariant->AllocateDefaultPins();

    static const FName Object_ParamName(TEXT("Object"));
    static const FName Variant_ParamName(TEXT("SoftVariant"));

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

void UK2Node_MakeSoftVariantFromSoftObject::AddMenuOptionsForStruct(
    FBlueprintActionDatabaseRegistrar &ActionRegistrar, Retro::IVariantRegistration &Registration) const {
    using FCustomizeDelegate = UBlueprintNodeSpawner::FCustomizeNodeDelegate;
    auto CustomizeCallback = [](UEdGraphNode *Node, bool, UScriptStruct *Output) {
        auto TypedNode = CastChecked<UK2Node_MakeSoftVariantFromSoftObject>(Node);
        TypedNode->Initialize(Output);
    };

    auto ActionKey = GetClass();
    auto SoftStruct = Registration.GetSoftStructType();
    auto Spawner = UBlueprintNodeSpawner::Create(ActionKey);
    check(Spawner != nullptr)
    Spawner->CustomizeNodeDelegate = FCustomizeDelegate::CreateStatic(CustomizeCallback, SoftStruct);
    ActionRegistrar.AddBlueprintAction(ActionKey, Spawner);
}

UEdGraphPin *UK2Node_MakeSoftVariantFromSoftObject::GetObjectPin() const {
    return FindPin(Retro::PN_Object);
}

TOptional<UClass &> UK2Node_MakeSoftVariantFromSoftObject::GetInputClass() const {
    // clang-format off
    return Retro::Optionals::OfNullable(GetObjectPin()) |
           Retro::Optionals::Transform<&UEdGraphPin::PinType>() |
           Retro::Optionals::Transform<&FEdGraphPinType::PinSubCategoryObject>() |
           Retro::Optionals::AndThen(Retro::DynamicCast<UClass>) |
           Retro::Optionals::To<TOptional>();
    // clang-format on
}

void UK2Node_MakeSoftVariantFromSoftObject::RefreshInputPinType() const {
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