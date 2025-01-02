// "Unreal Pokémon" created by Retro & Chill.

#include "RetroLib/Variants/K2Node_LoadVariantSynchronous.h"
#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintNodeSpawner.h"
#include "K2Node_CallFunction.h"
#include "KismetCompiler.h"
#include "RetroLib/Blueprints/BlueprintPins.h"
#include "RetroLib/Optionals/AndThen.h"
#include "RetroLib/Optionals/PtrOrNull.h"
#include "RetroLib/Optionals/Transform.h"
#include "RetroLib/Variants/VariantObjectStruct.h"
#include "RetroLib/Variants/VariantObjectUtilities.h"

void UK2Node_LoadVariantSynchronous::AllocateDefaultPins() {
    CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Execute);
    CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Wildcard, Retro::PN_SoftReference);
    CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Then);
    CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, Retro::PN_LoadFailed);
    CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Wildcard, UEdGraphSchema_K2::PN_ReturnValue);

    Super::AllocateDefaultPins();
}

void UK2Node_LoadVariantSynchronous::PostReconstructNode() {
    Super::PostReconstructNode();
    RefreshInputPin();
}

bool UK2Node_LoadVariantSynchronous::IsConnectionDisallowed(const UEdGraphPin *MyPin, const UEdGraphPin *OtherPin,
                                                            FString &OutReason) const {
    if ((MyPin != GetSoftReferencePin() && MyPin != GetResultPin()) ||
        MyPin->PinType.PinCategory != UEdGraphSchema_K2::PC_Wildcard) {
        return false;
    }

    bool bDisallowed = true;
    if (OtherPin->PinType.PinCategory == UEdGraphSchema_K2::PC_Struct) {
        auto &Registry = Retro::FVariantObjectStructRegistry::Get();
        auto Struct = Cast<UScriptStruct>(OtherPin->PinType.PinSubCategoryObject.Get());
        auto GetStructFunction = MyPin == GetSoftReferencePin() ? &Retro::IVariantRegistration::GetSoftStructType
                                                                : &Retro::IVariantRegistration::GetStructType;
        // clang-format off
        auto Result = Retro::Optionals::OfNullable(Struct) |
                      Retro::Optionals::AndThen<&Retro::FVariantObjectStructRegistry::GetVariantStructData>(Retro::TThis(std::ref(Registry))) |
                      Retro::Optionals::Transform(GetStructFunction) |
                      Retro::Optionals::PtrOrNull;
        // clang-format on
        bDisallowed = Struct == nullptr || Struct != Result;
    }

    if (bDisallowed) {
        OutReason = TEXT("Not a valid soft variant structure!");
    }
    return bDisallowed;
}

void UK2Node_LoadVariantSynchronous::NotifyPinConnectionListChanged(UEdGraphPin *Pin) {
    Super::NotifyPinConnectionListChanged(Pin);
    if (Pin == GetResultPin() || Pin == GetSoftReferencePin()) {
        RefreshInputPin();
    }
}

FText UK2Node_LoadVariantSynchronous::GetNodeTitle(ENodeTitleType::Type TitleType) const {
    return NSLOCTEXT("K2Node", "LoadVariantSynchronous_GetNodeTitle", "Load Synchronous");
}

FText UK2Node_LoadVariantSynchronous::GetTooltipText() const {
    return NSLOCTEXT("K2Node", "LoadVariantSynchronous_GetTooltipText",
                     "Synchronously load (if necessary) and return the asset object represented by this asset ptr");
}

bool UK2Node_LoadVariantSynchronous::IsNodePure() const {
    return false;
}

void UK2Node_LoadVariantSynchronous::GetMenuActions(FBlueprintActionDatabaseRegistrar &ActionRegistrar) const {
    auto ActionKey = GetClass();
    if (!ActionRegistrar.IsOpenForRegistration(ActionKey)) {
        return;
    }

    auto Spawner = UBlueprintNodeSpawner::Create(ActionKey);
    check(Spawner != nullptr)
    ActionRegistrar.AddBlueprintAction(ActionKey, Spawner);
}

void UK2Node_LoadVariantSynchronous::EarlyValidation(FCompilerResultsLog &MessageLog) const {
    Super::EarlyValidation(MessageLog);

    if (auto InputStruct = GetInputStruct(); !InputStruct.IsSet()) {
        MessageLog.Error(TEXT("Must have a valid connection to the input pin"));
    }

    if (auto OutputStruct = GetOutputStruct(); !OutputStruct.IsSet()) {
        MessageLog.Error(TEXT("Must have a valid connection to the output pin"));
    }
}

void UK2Node_LoadVariantSynchronous::ExpandNode(FKismetCompilerContext &CompilerContext, UEdGraph *SourceGraph) {
    Super::ExpandNode(CompilerContext, SourceGraph);

    // FUNCTION NODE
    const FName FunctionName = GET_FUNCTION_NAME_CHECKED(UVariantObjectUtilities, LoadSynchronous);
    auto CallCreateVariant = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
    CallCreateVariant->FunctionReference.SetExternalMember(FunctionName, UVariantObjectUtilities::StaticClass());
    CallCreateVariant->AllocateDefaultPins();

    static const FName SoftVariant_ParamName = "SoftVariant";
    static const FName Variant_ParamName = "Variant";

    auto ExecPin = FindPinChecked(UEdGraphSchema_K2::PN_Execute);
    auto ThenPin = FindPinChecked(UEdGraphSchema_K2::PN_Then);
    auto FailedPin = FindPinChecked(Retro::PN_LoadFailed);
    auto InputPin = GetSoftReferencePin();
    auto ReturnValuePin = GetResultPin();

    auto CallCreateExecPin = CallCreateVariant->FindPinChecked(UEdGraphSchema_K2::PN_Execute);
    auto CallCreateThenPin = CallCreateVariant->FindPinChecked(Retro::PN_True);
    auto CallCreateFailedPin = CallCreateVariant->FindPinChecked(Retro::PN_False);
    auto CallCreateSoftReferencePin = CallCreateVariant->FindPinChecked(SoftVariant_ParamName);
    auto CallCreateVariantPin = CallCreateVariant->FindPinChecked(Variant_ParamName);

    CompilerContext.MovePinLinksToIntermediate(*ExecPin, *CallCreateExecPin);
    CompilerContext.MovePinLinksToIntermediate(*ThenPin, *CallCreateThenPin);
    CompilerContext.MovePinLinksToIntermediate(*FailedPin, *CallCreateFailedPin);

    CallCreateSoftReferencePin->PinType = InputPin->PinType;
    CallCreateSoftReferencePin->PinType.PinSubCategoryObject = InputPin->PinType.PinSubCategoryObject;
    CompilerContext.MovePinLinksToIntermediate(*InputPin, *CallCreateSoftReferencePin);

    CallCreateVariantPin->PinType = ReturnValuePin->PinType;
    CallCreateVariantPin->PinType.PinSubCategoryObject = ReturnValuePin->PinType.PinSubCategoryObject;
    CompilerContext.MovePinLinksToIntermediate(*ReturnValuePin, *CallCreateVariantPin);

    BreakAllNodeLinks();
}

UEdGraphPin *UK2Node_LoadVariantSynchronous::GetSoftReferencePin() const {
    return FindPin(Retro::PN_SoftReference);
}

UEdGraphPin *UK2Node_LoadVariantSynchronous::GetResultPin() const {
    return FindPin(UEdGraphSchema_K2::PN_ReturnValue);
}

TOptional<UScriptStruct &> UK2Node_LoadVariantSynchronous::GetInputStruct() const {
    auto ObjectPin = GetSoftReferencePin();
    if (ObjectPin == nullptr || ObjectPin->PinType.PinCategory == UEdGraphSchema_K2::PC_Wildcard) {
        return nullptr;
    }

    return Cast<UScriptStruct>(ObjectPin->PinType.PinSubCategoryObject.Get());
}

TOptional<UScriptStruct &> UK2Node_LoadVariantSynchronous::GetOutputStruct() const {
    auto ObjectPin = GetResultPin();
    if (ObjectPin == nullptr || ObjectPin->PinType.PinCategory == UEdGraphSchema_K2::PC_Wildcard) {
        return nullptr;
    }

    return Cast<UScriptStruct>(ObjectPin->PinType.PinSubCategoryObject.Get());
}

void UK2Node_LoadVariantSynchronous::RefreshInputPin() const {
    auto InputPin = GetSoftReferencePin();
    auto OutputPin = GetResultPin();
    auto &Registry = Retro::FVariantObjectStructRegistry::Get();
    if (InputPin->LinkedTo.Num() > 0) {
        check(InputPin->LinkedTo.Num() == 1)
        auto Pin = InputPin->LinkedTo[0];
        InputPin->PinType = Pin->PinType;
        InputPin->PinType.PinSubCategoryObject = Pin->PinType.PinSubCategoryObject;

        auto Registration =
            Registry.GetVariantStructData(*CastChecked<UScriptStruct>(InputPin->PinType.PinSubCategoryObject.Get()));
        check(Registration.IsSet())

        OutputPin->PinType.PinCategory = UEdGraphSchema_K2::PC_Struct;
        OutputPin->PinType.PinSubCategoryObject = Registration->GetStructType();
    } else if (OutputPin->LinkedTo.Num() > 0) {
        auto Pin = OutputPin->LinkedTo[0];
        OutputPin->PinType = Pin->PinType;
        OutputPin->PinType.PinSubCategoryObject = Pin->PinType.PinSubCategoryObject;

        auto Registration =
            Registry.GetVariantStructData(*CastChecked<UScriptStruct>(OutputPin->PinType.PinSubCategoryObject.Get()));
        check(Registration.IsSet())

        InputPin->PinType.PinCategory = UEdGraphSchema_K2::PC_Struct;
        InputPin->PinType.PinSubCategoryObject = Registration->GetSoftStructType();
    } else {
        InputPin->PinType.PinCategory = UEdGraphSchema_K2::PC_Wildcard;
        InputPin->PinType.PinSubCategoryObject = nullptr;
        OutputPin->PinType.PinCategory = UEdGraphSchema_K2::PC_Wildcard;
        OutputPin->PinType.PinSubCategoryObject = nullptr;
    }
}