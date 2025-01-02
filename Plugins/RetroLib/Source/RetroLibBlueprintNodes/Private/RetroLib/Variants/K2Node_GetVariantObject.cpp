// "Unreal Pokémon" created by Retro & Chill.

#include "RetroLib/Variants/K2Node_GetVariantObject.h"

#include <RetroLib/Optionals/Transform.h>

#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintNodeSpawner.h"
#include "K2Node_CallFunction.h"
#include "KismetCompiler.h"
#include "RetroLib/Blueprints/BlueprintPins.h"
#include "RetroLib/Optionals/Transform.h"
#include "RetroLib/Optionals/PtrOrNull.h"
#include "RetroLib/Optionals/AndThen.h"
#include "RetroLib/Variants/VariantObjectStruct.h"
#include "RetroLib/Variants/VariantObjectUtilities.h"

void UK2Node_GetVariantObject::AllocateDefaultPins() {
    CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Wildcard, Retro::PN_Variant);
    CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Object, UObject::StaticClass(), UEdGraphSchema_K2::PN_ReturnValue);

    Super::AllocateDefaultPins();
}

void UK2Node_GetVariantObject::PostReconstructNode() {
    Super::PostReconstructNode();
    RefreshInputPin();
}

bool UK2Node_GetVariantObject::IsConnectionDisallowed(const UEdGraphPin *MyPin, const UEdGraphPin *OtherPin,
                                                      FString &OutReason) const {
    if (MyPin != GetVariantPin() || MyPin->PinType.PinCategory != UEdGraphSchema_K2::PC_Wildcard) {
        return false;
    }

    bool bDisallowed = true;
    if (OtherPin->PinType.PinCategory == UEdGraphSchema_K2::PC_Struct) {
        auto &Registry = Retro::FVariantObjectStructRegistry::Get();
        auto Struct = Cast<UScriptStruct>(OtherPin->PinType.PinSubCategoryObject.Get());
        // clang-format off
        auto Result = Retro::Optionals::OfNullable<TOptional>(Struct) |
                      Retro::Optionals::AndThen<&Retro::FVariantObjectStructRegistry::GetVariantStructData>(Retro::TThis(&Registry)) |
                      Retro::Optionals::Transform<&Retro::IVariantRegistration::GetStructType>() |
                      Retro::Optionals::PtrOrNull;
        // clang-format on
        bDisallowed = Struct == nullptr || Struct != Result;
    }

    if (bDisallowed) {
        OutReason = TEXT("Not a valid variant structure!");
    }
    return bDisallowed;
}

void UK2Node_GetVariantObject::NotifyPinConnectionListChanged(UEdGraphPin *Pin) {
    Super::NotifyPinConnectionListChanged(Pin);

    if (Pin != GetVariantPin()) {
        return;
    }

    RefreshInputPin();
}

FText UK2Node_GetVariantObject::GetNodeTitle(ENodeTitleType::Type TitleType) const {
    return NSLOCTEXT("K2Node", "GetVariantObject_GetNodeTitle", "Get Object from Variant");
}

FText UK2Node_GetVariantObject::GetCompactNodeTitle() const {
    return NSLOCTEXT("K2Node", "GetVariantObject_GetCompactNodeTitle", "GET");
}

bool UK2Node_GetVariantObject::ShouldDrawCompact() const {
    return true;
}

FText UK2Node_GetVariantObject::GetTooltipText() const {
    return NSLOCTEXT("K2Node", "GetVariantObject_GetVariantObject",
                     "Take the input variant struct object and convert it into an object reference.");
}

void UK2Node_GetVariantObject::GetMenuActions(FBlueprintActionDatabaseRegistrar &ActionRegistrar) const {
    auto ActionKey = GetClass();
    if (!ActionRegistrar.IsOpenForRegistration(ActionKey)) {
        return;
    }

    auto Spawner = UBlueprintNodeSpawner::Create(ActionKey);
    check(Spawner != nullptr)
    ActionRegistrar.AddBlueprintAction(ActionKey, Spawner);
}

void UK2Node_GetVariantObject::EarlyValidation(FCompilerResultsLog &MessageLog) const {
    Super::EarlyValidation(MessageLog);

    if (auto InputStruct = GetInputStruct(); !InputStruct.IsSet()) {
        MessageLog.Error(TEXT("Must have a valid connection to the input pin"));
    }
}

void UK2Node_GetVariantObject::ExpandNode(FKismetCompilerContext &CompilerContext, UEdGraph *SourceGraph) {
    Super::ExpandNode(CompilerContext, SourceGraph);

    // FUNCTION NODE
    const FName FunctionName = GET_FUNCTION_NAME_CHECKED(UVariantObjectUtilities, GetObjectFromVariant);
    auto CallCreateVariant = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
    CallCreateVariant->FunctionReference.SetExternalMember(FunctionName, UVariantObjectUtilities::StaticClass());
    CallCreateVariant->AllocateDefaultPins();

    static const FName Variant_ParamName = "Variant";

    auto InputPin = FindPinChecked(Retro::PN_Variant);
    auto ReturnValuePin = FindPinChecked(UEdGraphSchema_K2::PN_ReturnValue);

    auto CallCreateVariantPin = CallCreateVariant->FindPinChecked(Variant_ParamName);
    auto CallCreateObjectPin = CallCreateVariant->FindPinChecked(UEdGraphSchema_K2::PN_ReturnValue);

    CallCreateVariantPin->PinType = InputPin->PinType;
    CallCreateVariantPin->PinType.PinSubCategoryObject = InputPin->PinType.PinSubCategoryObject;
    CompilerContext.MovePinLinksToIntermediate(*InputPin, *CallCreateVariantPin);

    CallCreateObjectPin->PinType = ReturnValuePin->PinType;
    CallCreateObjectPin->PinType.PinSubCategoryObject = ReturnValuePin->PinType.PinSubCategoryObject;
    CompilerContext.MovePinLinksToIntermediate(*ReturnValuePin, *CallCreateObjectPin);

    BreakAllNodeLinks();
}

UEdGraphPin *UK2Node_GetVariantObject::GetVariantPin() const {
    return FindPin(Retro::PN_Variant);
}

TOptional<UScriptStruct &> UK2Node_GetVariantObject::GetInputStruct() const {
    auto ObjectPin = GetVariantPin();
    if (ObjectPin == nullptr || ObjectPin->PinType.PinCategory == UEdGraphSchema_K2::PC_Wildcard) {
        return nullptr;
    }

    return Cast<UScriptStruct>(ObjectPin->PinType.PinSubCategoryObject.Get());
}

void UK2Node_GetVariantObject::RefreshInputPin() const {
    if (auto ObjectPin = GetVariantPin(); ObjectPin->LinkedTo.Num() > 0) {
        check(ObjectPin->LinkedTo.Num() == 1)
        auto Pin = ObjectPin->LinkedTo[0];
        ObjectPin->PinType = Pin->PinType;
        ObjectPin->PinType.PinSubCategoryObject = Pin->PinType.PinSubCategoryObject;
    } else {
        ObjectPin->PinType.PinCategory = UEdGraphSchema_K2::PC_Wildcard;
        ObjectPin->PinType.PinSubCategoryObject = nullptr;
    }
}