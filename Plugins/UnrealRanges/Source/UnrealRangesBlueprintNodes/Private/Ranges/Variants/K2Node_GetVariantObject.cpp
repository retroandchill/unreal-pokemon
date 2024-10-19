// "Unreal Pokémon" created by Retro & Chill.


#include "Ranges/Variants/K2Node_GetVariantObject.h"
#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintNodeSpawner.h"
#include "K2Node_CallFunction.h"
#include "KismetCompiler.h"
#include "Ranges/Variants/VariantObjectUtilities.h"

void UK2Node_GetVariantObject::Initialize(UScriptStruct *Input) {
    InputType = Input;
}

void UK2Node_GetVariantObject::AllocateDefaultPins() {
    CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Struct,
              InputType != nullptr ? InputType.Get() : nullptr,
              InputType != nullptr ? InputType->GetFName() : FName("Struct"));
    CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Object, UEdGraphSchema_K2::PN_ReturnValue);

    Super::AllocateDefaultPins();
}

FText UK2Node_GetVariantObject::GetNodeTitle(ENodeTitleType::Type TitleType) const {
    return NSLOCTEXT("K2Node", "GetVariantObject_GetNodeTitle", "Get Object");
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

void UK2Node_GetVariantObject::ExpandNode(FKismetCompilerContext &CompilerContext, UEdGraph *SourceGraph) {
    Super::ExpandNode(CompilerContext, SourceGraph);

    // FUNCTION NODE
    const FName FunctionName = GET_FUNCTION_NAME_CHECKED(UVariantObjectUtilities, GetObjectFromVariant);
    auto CallCreateVariant = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
    CallCreateVariant->FunctionReference.SetExternalMember(FunctionName, UVariantObjectUtilities::StaticClass());
    CallCreateVariant->AllocateDefaultPins();

    static const FName Variant_ParamName = "Variant";

    auto InputPin = FindPinChecked(InputType != nullptr ? InputType->GetFName() : FName("Struct"));
    auto ReturnValuePin = FindPinChecked(UEdGraphSchema_K2::PN_ReturnValue);

    auto CallCreateVariantPin = CallCreateVariant->FindPinChecked(Variant_ParamName);
    auto CallCreateObjectPin = CallCreateVariant->FindPinChecked(UEdGraphSchema_K2::PN_ReturnValue);

    CallCreateObjectPin->PinType = InputPin->PinType;
    CallCreateObjectPin->PinType.PinSubCategoryObject = InputPin->PinType.PinSubCategoryObject;
    CompilerContext.MovePinLinksToIntermediate(*InputPin, *CallCreateObjectPin);

    CallCreateVariantPin->PinType = ReturnValuePin->PinType;
    CallCreateVariantPin->PinType.PinSubCategoryObject = ReturnValuePin->PinType.PinSubCategoryObject;
    CompilerContext.MovePinLinksToIntermediate(*ReturnValuePin, *CallCreateVariantPin);

    BreakAllNodeLinks();
}

void UK2Node_GetVariantObject::AddMenuOptionsForStruct(FBlueprintActionDatabaseRegistrar &ActionRegistrar,
    UE::Ranges::IVariantRegistration &Registration) const {
    using FCustomizeDelegate = UBlueprintNodeSpawner::FCustomizeNodeDelegate;
    auto CustomizeCallback = [](UEdGraphNode *Node, bool, UScriptStruct *Input) {
        auto TypedNode = CastChecked<UK2Node_GetVariantObject>(Node);
        TypedNode->Initialize(Input);
    };

    auto ActionKey = GetClass();
    auto Struct = Registration.GetStructType();
    auto Spawner = UBlueprintNodeSpawner::Create(ActionKey);
    check(Spawner != nullptr);
    Spawner->CustomizeNodeDelegate = FCustomizeDelegate::CreateLambda(CustomizeCallback, Struct);
    ActionRegistrar.AddBlueprintAction(ActionKey, Spawner);
}