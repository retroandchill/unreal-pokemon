// "Unreal Pokémon" created by Retro & Chill.

#include "RetroLib/Variants/K2Node_CastObjectToVariant.h"
#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintNodeSpawner.h"
#include "K2Node_CallFunction.h"
#include "KismetCompiler.h"
#include "RetroLib/Blueprints/BlueprintPins.h"
#include "RetroLib/Variants/VariantObjectStruct.h"
#include "RetroLib/Variants/VariantObjectUtilities.h"

void UK2Node_CastObjectToVariant::Initialize(UScriptStruct *Output) {
    OutputType = Output;
}

FText UK2Node_CastObjectToVariant::GetNodeTitle(ENodeTitleType::Type TitleType) const {
    auto StructName =
        OutputType != nullptr ? OutputType->GetDisplayNameText() : FText::FromStringView(TEXT("<<INVALID>>"));
    return FText::FormatNamed(NSLOCTEXT("K2Node", "CastObjectToVariant_GetNodeTitle", "Cast to {Output}"),
                              TEXT("Output"), StructName);
}

FText UK2Node_CastObjectToVariant::GetTooltipText() const {
    auto StructName =
        OutputType != nullptr ? OutputType->GetDisplayNameText() : FText::FromStringView(TEXT("<<INVALID>>"));
    return FText::FormatNamed(
        NSLOCTEXT("K2Node", "CastObjectToVariant_GetNodeTitle", "Attempt to cast an object cast to {Output}"),
        TEXT("Output"), StructName);
}

void UK2Node_CastObjectToVariant::CreateInputAndOutputPins() {
    CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Object, UObject::StaticClass(), Retro::PN_Object);
    CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Struct, OutputType != nullptr ? OutputType.Get() : nullptr,
              UEdGraphSchema_K2::PN_ReturnValue);
}

void UK2Node_CastObjectToVariant::AddMenuOptionsForStruct(FBlueprintActionDatabaseRegistrar &ActionRegistrar,
                                                          Retro::IVariantRegistration &Registration) const {
    using FCustomizeDelegate = UBlueprintNodeSpawner::FCustomizeNodeDelegate;
    auto CustomizeCallback = [](UEdGraphNode *Node, bool, UScriptStruct *Output) {
        auto TypedNode = CastChecked<UK2Node_CastObjectToVariant>(Node);
        TypedNode->Initialize(Output);
    };

    auto ActionKey = GetClass();
    auto Struct = Registration.GetStructType();
    auto Spawner = UBlueprintNodeSpawner::Create(ActionKey);
    check(Spawner != nullptr)
    Spawner->CustomizeNodeDelegate = FCustomizeDelegate::CreateLambda(CustomizeCallback, Struct);
    ActionRegistrar.AddBlueprintAction(ActionKey, Spawner);
}

UEdGraphPin *UK2Node_CastObjectToVariant::GetInputPin() const {
    return FindPin(Retro::PN_Object);
}

UEdGraphPin *UK2Node_CastObjectToVariant::GetOutputPin() const {
    return FindPin(UEdGraphSchema_K2::PN_ReturnValue);
}

UK2Node_VariantCastBase::FCastFunctionInfo
UK2Node_CastObjectToVariant::GetPerformCastNode(FKismetCompilerContext &CompilerContext, UEdGraph *SourceGraph) {
    const FName FunctionName = GET_FUNCTION_NAME_CHECKED(UVariantObjectUtilities, CreateVariantFromObjectChecked);
    auto CallCreateVariant = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
    CallCreateVariant->FunctionReference.SetExternalMember(FunctionName, UVariantObjectUtilities::StaticClass());
    CallCreateVariant->AllocateDefaultPins();

    static const FName Object_ParamName(TEXT("Object"));
    static const FName Variant_ParamName(TEXT("Variant"));
    return {CallCreateVariant, CallCreateVariant->FindPinChecked(Object_ParamName),
            CallCreateVariant->FindPinChecked(Variant_ParamName)};
}