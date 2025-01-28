// "Unreal Pokémon" created by Retro & Chill.

#include "RetroLib/Variants/K2Node_CastSoftVariantToSoftVariant.h"
#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintNodeSpawner.h"
#include "K2Node_CallFunction.h"
#include "KismetCompiler.h"
#include "RetroLib/Blueprints/BlueprintPins.h"
#include "RetroLib/Variants/VariantObjectStruct.h"
#include "RetroLib/Variants/VariantObjectUtilities.h"

class UK2Node_CallFunction;

void UK2Node_CastSoftVariantToSoftVariant::Initialize(UScriptStruct *Input, UScriptStruct *Output) {
    InputType = Input;
    OutputType = Output;
}

FText UK2Node_CastSoftVariantToSoftVariant::GetNodeTitle(ENodeTitleType::Type TitleType) const {
    auto StructName =
        OutputType != nullptr ? OutputType->GetDisplayNameText() : FText::FromStringView(TEXT("<<INVALID>>"));
    return FText::FormatNamed(NSLOCTEXT("K2Node", "CastSoftVariantToSoftVariant_GetNodeTitle", "Cast to {Output}"),
                              TEXT("Output"), StructName);
}

FText UK2Node_CastSoftVariantToSoftVariant::GetTooltipText() const {
    auto StructName =
        OutputType != nullptr ? OutputType->GetDisplayNameText() : FText::FromStringView(TEXT("<<INVALID>>"));
    return FText::FormatNamed(
        NSLOCTEXT("K2Node", "CastSoftVariantToSoftVariant_GetNodeTitle", "Attempt to cast an object cast to {Output}"),
        TEXT("Output"), StructName);
}

void UK2Node_CastSoftVariantToSoftVariant::CreateInputAndOutputPins() {
    CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Struct, InputType != nullptr ? InputType.Get() : nullptr,
              Retro::PN_SoftReference);
    CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Struct, OutputType != nullptr ? OutputType.Get() : nullptr,
              UEdGraphSchema_K2::PN_ReturnValue);
}

void UK2Node_CastSoftVariantToSoftVariant::AddMenuOptionsForStruct(FBlueprintActionDatabaseRegistrar &ActionRegistrar,
                                                                   Retro::IVariantRegistration &Registration) const {
    using FCustomizeDelegate = UBlueprintNodeSpawner::FCustomizeNodeDelegate;
    auto CustomizeCallback = [](UEdGraphNode *Node, bool, UScriptStruct *Input, UScriptStruct *Output) {
        auto TypedNode = CastChecked<UK2Node_CastSoftVariantToSoftVariant>(Node);
        TypedNode->Initialize(Input, Output);
    };

    auto ActionKey = GetClass();
    auto SourceStruct = Registration.GetSoftStructType();
    TSet<UScriptStruct *> Seen;
    for (const auto &Conversion : Registration.GetAllConversions()) {
        auto DestStruct = Conversion.GetDestSoftStructType();
        if (Seen.Contains(DestStruct)) {
            continue;
        }

        Seen.Add(DestStruct);
        auto Spawner = UBlueprintNodeSpawner::Create(ActionKey);
        check(Spawner != nullptr)
        Spawner->CustomizeNodeDelegate = FCustomizeDelegate::CreateLambda(CustomizeCallback, SourceStruct, DestStruct);
        ActionRegistrar.AddBlueprintAction(ActionKey, Spawner);
    }
}

UEdGraphPin *UK2Node_CastSoftVariantToSoftVariant::GetInputPin() const {
    return FindPin(Retro::PN_SoftReference);
}

UEdGraphPin *UK2Node_CastSoftVariantToSoftVariant::GetOutputPin() const {
    return FindPin(UEdGraphSchema_K2::PN_ReturnValue);
}

UK2Node_VariantCastBase::FCastFunctionInfo
UK2Node_CastSoftVariantToSoftVariant::GetPerformCastNode(FKismetCompilerContext &CompilerContext,
                                                         UEdGraph *SourceGraph) {
    const FName FunctionName = GET_FUNCTION_NAME_CHECKED(UVariantObjectUtilities, ConvertSoftVariantObject);
    auto CallCreateVariant = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
    CallCreateVariant->FunctionReference.SetExternalMember(FunctionName, UVariantObjectUtilities::StaticClass());
    CallCreateVariant->AllocateDefaultPins();

    static const FName Input_ParamName(TEXT("From"));
    static const FName Output_ParamName(TEXT("To"));
    return {CallCreateVariant, CallCreateVariant->FindPinChecked(Input_ParamName),
            CallCreateVariant->FindPinChecked(Output_ParamName)};
}