// "Unreal Pokémon" created by Retro & Chill.


#include "Ranges/Variants/K2Node_CastObjectToVariant.h"
#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintNodeSpawner.h"
#include "K2Node_CallFunction.h"
#include "KismetCompiler.h"
#include "Ranges/Variants/VariantObjectStruct.h"
#include "Ranges/Variants/VariantObjectUtilities.h"
#include "Ranges/Views/Filter.h"

void UK2Node_CastObjectToVariant::Initialize(UScriptStruct *Output) {
    OutputType = Output;
}

void UK2Node_CastObjectToVariant::AllocateDefaultPins() {
    CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Execute);
    CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Object, "Object");
    CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Then);
    CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_CastFailed);
    CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Struct,
              (OutputType != nullptr ? OutputType.Get() : FTableRowBase::StaticStruct()),
              UEdGraphSchema_K2::PN_ReturnValue);

    Super::AllocateDefaultPins();
}

FText UK2Node_CastObjectToVariant::GetNodeTitle(ENodeTitleType::Type TitleType) const {
    auto StructName = OutputType != nullptr
                          ? OutputType->GetDisplayNameText()
                          : FText::FromStringView(TEXT("<<INVALID>>"));
    return FText::FormatNamed(NSLOCTEXT("K2Node", "CastObjectToVariant_GetNodeTitle", "Cast to {Output}"),
                              TEXT("Output"), StructName);
}

FText UK2Node_CastObjectToVariant::GetTooltipText() const {
    auto StructName = OutputType != nullptr
                          ? OutputType->GetDisplayNameText()
                          : FText::FromStringView(TEXT("<<INVALID>>"));
    return FText::FormatNamed(NSLOCTEXT("K2Node", "CastObjectToVariant_GetNodeTitle",
        "Attempt to cast an object cast to {Output}"), TEXT("Output"), StructName);
}

bool UK2Node_CastObjectToVariant::IsNodePure() const {
    return false;
}

FText UK2Node_CastObjectToVariant::GetMenuCategory() const {
    return NSLOCTEXT("K2Node", "CastObjectToVariant_GetMenuCategory", "Variants");
}


FLinearColor UK2Node_CastObjectToVariant::GetNodeTitleColor() const {
    return FLinearColor(0.0f, 0.55f, 0.62f);
}

FSlateIcon UK2Node_CastObjectToVariant::GetIconAndTint(FLinearColor &OutColor) const {
    static FSlateIcon Icon(FAppStyle::GetAppStyleSetName(), "GraphEditor.Cast_16x");
    return Icon;
}

void UK2Node_CastObjectToVariant::GetMenuActions(FBlueprintActionDatabaseRegistrar &ActionRegistrar) const {
    auto CustomizeCallback = [](UEdGraphNode *Node, [[maybe_unused]] bool bIsTemplateNode,
                                UScriptStruct *Output) {
        auto TypedNode = CastChecked<UK2Node_CastObjectToVariant>(Node);
        TypedNode->Initialize(Output);
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

        auto Spawner = UBlueprintNodeSpawner::Create(ActionKey);
        check(Spawner != nullptr);
        Spawner->CustomizeNodeDelegate = UBlueprintNodeSpawner::FCustomizeNodeDelegate::CreateStatic(
            CustomizeCallback, Struct);
        ActionRegistrar.AddBlueprintAction(ActionKey, Spawner);
    }
}

void UK2Node_CastObjectToVariant::ExpandNode(FKismetCompilerContext &CompilerContext, UEdGraph *SourceGraph) {
    Super::ExpandNode(CompilerContext, SourceGraph);

    // FUNCTION NODE
    const FName FunctionName = GET_FUNCTION_NAME_CHECKED(UVariantObjectUtilities, CreateVariantFromObjectChecked);
    auto CallCreateVariant = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
    CallCreateVariant->FunctionReference.SetExternalMember(FunctionName, UVariantObjectUtilities::StaticClass());
    CallCreateVariant->AllocateDefaultPins();
    
    static const FName Object_ParamName(TEXT("Object"));
    static const FName Variant_ParamName(TEXT("Variant"));

    auto ExecPin = FindPinChecked(UEdGraphSchema_K2::PN_Execute);
    auto InputPin = FindPinChecked(FName("Object"));
    auto ReturnValuePin = FindPinChecked(UEdGraphSchema_K2::PN_ReturnValue);
    auto ThenPin = FindPinChecked(UEdGraphSchema_K2::PN_Then);
    auto CastFailedPin = FindPinChecked(UEdGraphSchema_K2::PN_CastFailed);

    auto CallCreateExecPin = CallCreateVariant->FindPinChecked(UEdGraphSchema_K2::PN_Execute);
    auto CallCreateObjectPin = CallCreateVariant->FindPinChecked(Object_ParamName);
    auto CallCreateVariantPin = CallCreateVariant->FindPinChecked(Variant_ParamName);
    auto CallCreateTruePin = CallCreateVariant->FindPinChecked(FName("True"));
    auto CallCreateFalsePin = CallCreateVariant->FindPinChecked(FName("False"));

    CompilerContext.MovePinLinksToIntermediate(*ExecPin, *CallCreateExecPin);
    CompilerContext.MovePinLinksToIntermediate(*InputPin, *CallCreateObjectPin);

    CallCreateVariantPin->PinType = ReturnValuePin->PinType;
    CallCreateVariantPin->PinType.PinSubCategoryObject = ReturnValuePin->PinType.PinSubCategoryObject;
    CompilerContext.MovePinLinksToIntermediate(*ReturnValuePin, *CallCreateVariantPin);
    CompilerContext.MovePinLinksToIntermediate(*ThenPin, *CallCreateTruePin);
    CompilerContext.MovePinLinksToIntermediate(*CastFailedPin, *CallCreateFalsePin);

    BreakAllNodeLinks();
}