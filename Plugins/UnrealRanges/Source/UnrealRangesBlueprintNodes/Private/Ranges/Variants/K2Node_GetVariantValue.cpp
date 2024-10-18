// "Unreal Pokémon" created by Retro & Chill.


#include "Ranges/Variants/K2Node_GetVariantValue.h"
#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintNodeSpawner.h"
#include "K2Node_CallFunction.h"
#include "KismetCompiler.h"
#include "Ranges/Variants/VariantObjectStruct.h"
#include "Ranges/Variants/VariantObjectUtilities.h"
#include "Ranges/Views/Filter.h"

void UK2Node_GetVariantValue::Initialize(UScriptStruct *Input, UClass *Output) {
    InputType = Input;
    OutputType = Output;
}

void UK2Node_GetVariantValue::AllocateDefaultPins() {
    CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Execute);
    CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Struct,
              (InputType != nullptr ? InputType.Get() : FTableRowBase::StaticStruct()),
              InputType != nullptr ? InputType->GetFName() : FName("Struct"));
    CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Then);
    CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_CastFailed);
    auto OutputPinType = OutputType != nullptr && OutputType->HasAnyClassFlags(CLASS_Interface)
                            ? UEdGraphSchema_K2::PC_Interface
                            : UEdGraphSchema_K2::PC_Object;
    auto OutputPinSubclass = OutputType != nullptr ? OutputType.Get() : UObject::StaticClass();
    CreatePin(EGPD_Output, OutputPinType, OutputPinSubclass,
        OutputType != nullptr ? OutputType->GetFName() : FName("Object"));

    Super::AllocateDefaultPins();
}

FText UK2Node_GetVariantValue::GetNodeTitle(ENodeTitleType::Type TitleType) const {
    auto StructName = OutputType != nullptr
                          ? OutputType->GetDisplayNameText()
                          : FText::FromStringView(TEXT("<<INVALID>>"));
    return FText::FormatNamed(NSLOCTEXT("K2Node", "GetVariantValue_GetNodeTitle", "Cast to {Output}"),
                              TEXT("Output"), StructName);
}

FText UK2Node_GetVariantValue::GetTooltipText() const {
    auto ClassName = InputType != nullptr
                         ? InputType->GetDisplayNameText()
                         : FText::FromStringView(TEXT("<<INVALID>>"));
    auto StructName = OutputType != nullptr
                          ? OutputType->GetDisplayNameText()
                          : FText::FromStringView(TEXT("<<INVALID>>"));
    return FText::FormatNamed(NSLOCTEXT("K2Node", "GetVariantValue_GetTooltipText",
                                        "Attempt to get an object of type {Output} from the provided {Input} structure"),
                              TEXT("Input"), StructName, TEXT("Output"), ClassName);
}

bool UK2Node_GetVariantValue::IsNodePure() const {
    return false;
}

FLinearColor UK2Node_GetVariantValue::GetNodeTitleColor() const {
    return FLinearColor(0.0f, 0.55f, 0.62f);
}

FText UK2Node_GetVariantValue::GetMenuCategory() const {
    return NSLOCTEXT("K2Node", "GetVariantValue_GetMenuCategory", "Variants");
}

FSlateIcon UK2Node_GetVariantValue::GetIconAndTint(FLinearColor &OutColor) const {
    static FSlateIcon Icon(FAppStyle::GetAppStyleSetName(), "GraphEditor.Cast_16x");
    return Icon;
}

void UK2Node_GetVariantValue::GetMenuActions(FBlueprintActionDatabaseRegistrar &ActionRegistrar) const {
    auto CustomizeCallback = [](UEdGraphNode *Node, [[maybe_unused]] bool bIsTemplateNode,
                                UScriptStruct *Input, UClass *Output) {
        auto TypedNode = CastChecked<UK2Node_GetVariantValue>(Node);
        TypedNode->Initialize(Input, Output);
    };

    auto ActionKey = GetClass();
    if (!ActionRegistrar.IsOpenForRegistration(ActionKey)) {
        return;
    }

    auto &Registry = UE::Ranges::FVariantObjectStructRegistry::Get();
    // clang-format off
    auto AllData = Registry.GetAllRegisteredStructs() |
                   UE::Ranges::Filter([](const UE::Ranges::IVariantRegistration &StructType) {
                       return UEdGraphSchema_K2::IsAllowableBlueprintVariableType(
                           StructType.GetStructType(), true);
                   });
    // clang-format on
    for (TSet<UScriptStruct *> Seen; auto &Registration : AllData) {
        auto Struct = Registration.GetStructType();
        if (Seen.Contains(Struct)) {
            continue;
        }
        Seen.Add(Struct);

        for (auto Classes = Registration.GetValidClasses(); auto Class : Classes) {
            auto Spawner = UBlueprintNodeSpawner::Create(ActionKey);
            check(Spawner != nullptr);
            Spawner->CustomizeNodeDelegate = UBlueprintNodeSpawner::FCustomizeNodeDelegate::CreateStatic(
                CustomizeCallback, Struct, Class);
            ActionRegistrar.AddBlueprintAction(ActionKey, Spawner);
        }
    }
}

void UK2Node_GetVariantValue::ExpandNode(FKismetCompilerContext &CompilerContext, UEdGraph *SourceGraph) {
    Super::ExpandNode(CompilerContext, SourceGraph);

    // FUNCTION NODE
    const FName FunctionName = GET_FUNCTION_NAME_CHECKED(UVariantObjectUtilities, GetObjectFromVariantChecked);
    auto CallCreateVariant = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
    CallCreateVariant->FunctionReference.SetExternalMember(FunctionName, UVariantObjectUtilities::StaticClass());
    CallCreateVariant->AllocateDefaultPins();

    static const FName Class_ParamName(TEXT("Class"));
    static const FName Variant_ParamName(TEXT("Variant"));
    static const FName Object_ParamName(TEXT("Object"));

    auto ExecPin = FindPinChecked(UEdGraphSchema_K2::PN_Execute);
    auto InputPin = FindPinChecked(FName("Object"));
    auto ReturnValuePin = FindPinChecked(OutputType->GetFName());
    auto ThenPin = FindPinChecked(UEdGraphSchema_K2::PN_Then);
    auto CastFailedPin = FindPinChecked(UEdGraphSchema_K2::PN_CastFailed);

    auto CallCreateExecPin = CallCreateVariant->FindPinChecked(UEdGraphSchema_K2::PN_Execute);
    auto CallCreateClassPin = CallCreateVariant->FindPinChecked(Class_ParamName);
    auto CallCreateObjectPin = CallCreateVariant->FindPinChecked(Object_ParamName);
    auto CallCreateVariantPin = CallCreateVariant->FindPinChecked(Variant_ParamName);
    auto CallCreateTruePin = CallCreateVariant->FindPinChecked(FName("True"));
    auto CallCreateFalsePin = CallCreateVariant->FindPinChecked(FName("False"));

    CompilerContext.MovePinLinksToIntermediate(*ExecPin, *CallCreateExecPin);
    CallCreateClassPin->DefaultObject = OutputType;
    
    CallCreateVariantPin->PinType.PinSubCategoryObject = ReturnValuePin->PinType.PinSubCategoryObject;
    CompilerContext.MovePinLinksToIntermediate(*InputPin, *CallCreateVariantPin);

    CallCreateVariantPin->PinType = ReturnValuePin->PinType;
    CallCreateVariantPin->PinType.PinSubCategoryObject = InputPin->PinType.PinSubCategoryObject;
    CompilerContext.MovePinLinksToIntermediate(*ReturnValuePin, *CallCreateObjectPin);
    CompilerContext.MovePinLinksToIntermediate(*ThenPin, *CallCreateTruePin);
    CompilerContext.MovePinLinksToIntermediate(*CastFailedPin, *CallCreateFalsePin);

    BreakAllNodeLinks();
}