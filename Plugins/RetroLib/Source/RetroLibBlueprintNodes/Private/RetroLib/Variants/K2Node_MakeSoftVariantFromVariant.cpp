// "Unreal Pokémon" created by Retro & Chill.

#include "RetroLib/Variants/K2Node_MakeSoftVariantFromVariant.h"
#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintNodeSpawner.h"
#include "K2Node_CallFunction.h"
#include "KismetCompiler.h"
#include "RetroLib/Variants/VariantObjectStruct.h"
#include "RetroLib/Variants/VariantObjectUtilities.h"

void UK2Node_MakeSoftVariantFromVariant::Initialize(UScriptStruct *HardReference, UScriptStruct *SoftReference) {
    HardReferenceType = HardReference;
    SoftReferenceType = SoftReference;
}

void UK2Node_MakeSoftVariantFromVariant::AllocateDefaultPins() {
    CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Struct,
              HardReferenceType != nullptr ? HardReferenceType.Get() : nullptr,
              HardReferenceType != nullptr ? HardReferenceType->GetFName() : FName("Struct"));
    CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Struct,
              SoftReferenceType != nullptr ? SoftReferenceType.Get() : nullptr, UEdGraphSchema_K2::PN_ReturnValue);

    Super::AllocateDefaultPins();
}

FText UK2Node_MakeSoftVariantFromVariant::GetNodeTitle(ENodeTitleType::Type TitleType) const {
    auto StructName = SoftReferenceType != nullptr ? SoftReferenceType->GetDisplayNameText()
                                                   : FText::FromStringView(TEXT("<<INVALID>>"));
    return FText::FormatNamed(NSLOCTEXT("K2Node", "MakeSoftVariantFromVariant_GetNodeTitle", "Make {Output}"),
                              TEXT("Output"), StructName);
}

FText UK2Node_MakeSoftVariantFromVariant::GetTooltipText() const {
    auto HardStructName = HardReferenceType != nullptr ? HardReferenceType->GetDisplayNameText()
                                                       : FText::FromStringView(TEXT("<<INVALID>>"));
    auto SoftStructName = SoftReferenceType != nullptr ? SoftReferenceType->GetDisplayNameText()
                                                       : FText::FromStringView(TEXT("<<INVALID>>"));
    return FText::FormatNamed(NSLOCTEXT("K2Node", "MakeSoftVariantFromVariant_GetTooltipText",
                                        "Make a new {Output} structure from the provided {Input} structure."),
                              TEXT("Input"), HardStructName, TEXT("Output"), SoftStructName);
}

void UK2Node_MakeSoftVariantFromVariant::ExpandNode(FKismetCompilerContext &CompilerContext, UEdGraph *SourceGraph) {
    Super::ExpandNode(CompilerContext, SourceGraph);

    // FUNCTION NODE
    const FName FunctionName = GET_FUNCTION_NAME_CHECKED(UVariantObjectUtilities, MakeSoftVariantFromVariant);
    auto CallCreateVariant = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
    CallCreateVariant->FunctionReference.SetExternalMember(FunctionName, UVariantObjectUtilities::StaticClass());
    CallCreateVariant->AllocateDefaultPins();

    static const FName Variant_ParamName = "Variant";
    static const FName SoftVariant_ParamName = "SoftVariant";

    auto InputPin = FindPinChecked(HardReferenceType != nullptr ? HardReferenceType->GetFName() : FName("Struct"));
    auto ReturnValuePin = FindPinChecked(UEdGraphSchema_K2::PN_ReturnValue);

    auto CallCreateVariantPin = CallCreateVariant->FindPinChecked(Variant_ParamName);
    auto CallCreateSoftAssetPin = CallCreateVariant->FindPinChecked(SoftVariant_ParamName);

    CallCreateVariantPin->PinType = InputPin->PinType;
    CallCreateVariantPin->PinType.PinSubCategoryObject = InputPin->PinType.PinSubCategoryObject;
    CompilerContext.MovePinLinksToIntermediate(*InputPin, *CallCreateVariantPin);

    CallCreateSoftAssetPin->PinType = ReturnValuePin->PinType;
    CallCreateSoftAssetPin->PinType.PinSubCategoryObject = ReturnValuePin->PinType.PinSubCategoryObject;
    CompilerContext.MovePinLinksToIntermediate(*ReturnValuePin, *CallCreateSoftAssetPin);

    BreakAllNodeLinks();
}

void UK2Node_MakeSoftVariantFromVariant::AddMenuOptionsForStruct(FBlueprintActionDatabaseRegistrar &ActionRegistrar,
                                                                 Retro::IVariantRegistration &Registration) const {
    using FCustomizeDelegate = UBlueprintNodeSpawner::FCustomizeNodeDelegate;
    auto CustomizeCallback = [](UEdGraphNode *Node, bool, UScriptStruct *Input, UScriptStruct *Output) {
        auto TypedNode = CastChecked<UK2Node_MakeSoftVariantFromVariant>(Node);
        TypedNode->Initialize(Input, Output);
    };

    auto ActionKey = GetClass();
    auto Spawner = UBlueprintNodeSpawner::Create(ActionKey);
    check(Spawner != nullptr)
    Spawner->CustomizeNodeDelegate = FCustomizeDelegate::CreateLambda(CustomizeCallback, Registration.GetStructType(),
                                                                      Registration.GetSoftStructType());
    ActionRegistrar.AddBlueprintAction(ActionKey, Spawner);
}