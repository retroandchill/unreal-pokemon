// "Unreal Pokémon" created by Retro & Chill.

#include "RetroLib/Variants/K2Node_GetVariantValue.h"
#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintNodeSpawner.h"
#include "K2Node_CallFunction.h"
#include "KismetCompiler.h"
#include "RetroLib/Variants/VariantObjectStruct.h"
#include "RetroLib/Variants/VariantObjectUtilities.h"

void UK2Node_GetVariantValue::Initialize(UScriptStruct *Input, UClass *Output) {
    InputType = Input;
    OutputType = Output;
}

FText UK2Node_GetVariantValue::GetNodeTitle(ENodeTitleType::Type TitleType) const {
    auto StructName =
        OutputType != nullptr ? OutputType->GetDisplayNameText() : FText::FromStringView(TEXT("<<INVALID>>"));
    return FText::FormatNamed(NSLOCTEXT("K2Node", "GetVariantValue_GetNodeTitle", "Cast to {Output}"), TEXT("Output"),
                              StructName);
}

FText UK2Node_GetVariantValue::GetTooltipText() const {
    auto ClassName =
        InputType != nullptr ? InputType->GetDisplayNameText() : FText::FromStringView(TEXT("<<INVALID>>"));
    auto StructName =
        OutputType != nullptr ? OutputType->GetDisplayNameText() : FText::FromStringView(TEXT("<<INVALID>>"));
    return FText::FormatNamed(
        NSLOCTEXT("K2Node", "GetVariantValue_GetTooltipText",
                  "Attempt to get an object of type {Output} from the provided {Input} structure"),
        TEXT("Input"), StructName, TEXT("Output"), ClassName);
}

void UK2Node_GetVariantValue::CreateInputAndOutputPins() {
    CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Struct, (InputType != nullptr ? InputType.Get() : nullptr),
              InputType != nullptr ? InputType->GetFName() : FName("Struct"));
    auto OutputPinType = OutputType != nullptr && OutputType->HasAnyClassFlags(CLASS_Interface)
                             ? UEdGraphSchema_K2::PC_Interface
                             : UEdGraphSchema_K2::PC_Object;
    auto OutputPinSubclass = OutputType != nullptr ? OutputType.Get() : UObject::StaticClass();
    CreatePin(EGPD_Output, OutputPinType, OutputPinSubclass,
              OutputType != nullptr ? OutputType->GetFName() : FName("Object"));
}

void UK2Node_GetVariantValue::AddMenuOptionsForStruct(FBlueprintActionDatabaseRegistrar &ActionRegistrar,
                                                      Retro::IVariantRegistration &Registration) const {
    using FCustomizeDelegate = UBlueprintNodeSpawner::FCustomizeNodeDelegate;

    auto Callback = [](UEdGraphNode *Node, bool, UClass *Class, UScriptStruct *Struct) {
        auto TypedNode = CastChecked<UK2Node_GetVariantValue>(Node);
        TypedNode->Initialize(Struct, Class);
    };

    auto ActionKey = GetClass();
    auto Struct = Registration.GetStructType();
    for (auto Classes = Registration.GetValidClasses(); auto Class : Classes) {
        auto Spawner = UBlueprintNodeSpawner::Create(ActionKey);
        check(Spawner != nullptr)
        Spawner->CustomizeNodeDelegate = FCustomizeDelegate::CreateLambda(Callback, Class, Struct);
        ActionRegistrar.AddBlueprintAction(ActionKey, Spawner);
    }
}

UEdGraphPin *UK2Node_GetVariantValue::GetInputPin() const {
    return FindPin(InputType != nullptr ? InputType->GetFName() : FName("Struct"));
}

UEdGraphPin *UK2Node_GetVariantValue::GetOutputPin() const {
    return FindPin(OutputType != nullptr ? OutputType->GetFName() : FName("Object"));
}

UK2Node_VariantCastBase::FCastFunctionInfo
UK2Node_GetVariantValue::GetPerformCastNode(FKismetCompilerContext &CompilerContext, UEdGraph *SourceGraph) {
    static const FName FunctionName = GET_FUNCTION_NAME_CHECKED(UVariantObjectUtilities, GetObjectFromVariantChecked);
    auto CallCreateVariant = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
    CallCreateVariant->FunctionReference.SetExternalMember(FunctionName, UVariantObjectUtilities::StaticClass());
    CallCreateVariant->AllocateDefaultPins();

    static const FName Variant_ParamName(TEXT("Variant"));
    static const FName Object_ParamName(TEXT("Object"));

    return {CallCreateVariant, CallCreateVariant->FindPinChecked(Variant_ParamName),
            CallCreateVariant->FindPinChecked(Object_ParamName)};
}

void UK2Node_GetVariantValue::MakeAdditionalPinLinks(UK2Node &CallPerformCast) const {
    static const FName Class_ParamName(TEXT("Class"));
    auto CallCreateClassPin = CallPerformCast.FindPinChecked(Class_ParamName);
    CallCreateClassPin->DefaultObject = OutputType;
}