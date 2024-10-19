// "Unreal Pokémon" created by Retro & Chill.


#include "Ranges/Variants/K2Node_MakeVariantObjectStruct.h"
#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintNodeSpawner.h"
#include "K2Node_CallFunction.h"
#include "KismetCompiler.h"
#include "Ranges/Variants/VariantObjectStruct.h"
#include "Ranges/Variants/VariantObjectUtilities.h"

void UK2Node_MakeVariantObjectStruct::Initialize(UClass *Input, UScriptStruct *Output) {
    InputType = Input;
    OutputType = Output;
}

void UK2Node_MakeVariantObjectStruct::AllocateDefaultPins() {
    auto InputPinType = InputType != nullptr && InputType->HasAnyClassFlags(CLASS_Interface)
                            ? UEdGraphSchema_K2::PC_Interface
                            : UEdGraphSchema_K2::PC_Object;
    auto InputPinSubclass = InputType != nullptr ? InputType.Get() : UObject::StaticClass();
    CreatePin(EGPD_Input, InputPinType, InputPinSubclass, InputType->GetFName());
    CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Struct,
              (OutputType != nullptr ? OutputType.Get() : FTableRowBase::StaticStruct()),
              UEdGraphSchema_K2::PN_ReturnValue);

    Super::AllocateDefaultPins();
}

FText UK2Node_MakeVariantObjectStruct::GetNodeTitle(ENodeTitleType::Type TitleType) const {
    auto ClassName = InputType != nullptr
                         ? InputType->GetDisplayNameText()
                         : FText::FromStringView(TEXT("<<INVALID>>"));
    auto StructName = OutputType != nullptr
                          ? OutputType->GetDisplayNameText()
                          : FText::FromStringView(TEXT("<<INVALID>>"));
    return FText::FormatNamed(NSLOCTEXT("K2Node", "MakeVariantObjectStruct_GetNodeTitle", "Make {Output} ({Input})"),
                              TEXT("Input"), ClassName, TEXT("Output"), StructName);
}

FText UK2Node_MakeVariantObjectStruct::GetTooltipText() const {
    auto ClassName = InputType != nullptr
                         ? InputType->GetDisplayNameText()
                         : FText::FromStringView(TEXT("<<INVALID>>"));
    auto StructName = OutputType != nullptr
                          ? OutputType->GetDisplayNameText()
                          : FText::FromStringView(TEXT("<<INVALID>>"));
    return FText::FormatNamed(NSLOCTEXT("K2Node", "MakeVariantObjectStruct_GetTooltipText",
                                        "Create a new {Output} structure from the source {Input} object."),
                              TEXT("Input"), ClassName, TEXT("Output"), StructName);
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

    auto InputPin = FindPinChecked(InputType->GetFName());
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
    UE::Ranges::IVariantRegistration &Registration) const {
    using FCustomizeDelegate = UBlueprintNodeSpawner::FCustomizeNodeDelegate;
    auto CustomizeCallback = [](UEdGraphNode *Node, bool,
                                UClass *Input, UScriptStruct *Output) {
        auto TypedNode = CastChecked<UK2Node_MakeVariantObjectStruct>(Node);
        TypedNode->Initialize(Input, Output);
    };

    auto ActionKey = GetClass();
    auto Struct = Registration.GetStructType();
    for (auto Classes = Registration.GetValidClasses(); auto Class : Classes) {
        auto Spawner = UBlueprintNodeSpawner::Create(ActionKey);
        check(Spawner != nullptr);
        Spawner->CustomizeNodeDelegate = FCustomizeDelegate::CreateStatic(CustomizeCallback, Class, Struct);
        ActionRegistrar.AddBlueprintAction(ActionKey, Spawner);
    }
}