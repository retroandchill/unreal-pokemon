// "Unreal Pokémon" created by Retro & Chill.


#include "Ranges/Variants/K2Node_MakeVariantObjectStruct.h"
#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintNodeSpawner.h"
#include "Ranges/Variants/VariantObjectStruct.h"
#include "Ranges/Views/Filter.h"
#include "Ranges/Views/Join.h"

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
    return Super::GetNodeTitle(TitleType);
}

FText UK2Node_MakeVariantObjectStruct::GetTooltipText() const {
    return Super::GetTooltipText();
}

bool UK2Node_MakeVariantObjectStruct::IsNodePure() const {
    return true;
}

FText UK2Node_MakeVariantObjectStruct::GetMenuCategory() const {
    return Super::GetMenuCategory();
}

FSlateIcon UK2Node_MakeVariantObjectStruct::GetIconAndTint(FLinearColor &OutColor) const {
    return Super::GetIconAndTint(OutColor);
}

void UK2Node_MakeVariantObjectStruct::GetMenuActions(FBlueprintActionDatabaseRegistrar &ActionRegistrar) const {
    auto CustomizeCallback = [](UEdGraphNode *Node, [[maybe_unused]] bool bIsTemplateNode,
                                UClass *Input, UScriptStruct *Output) {
        auto TypedNode = CastChecked<UK2Node_MakeVariantObjectStruct>(Node);
        TypedNode->Initialize(Input, Output);
    };

    auto ActionKey = GetClass();
    if (!ActionRegistrar.IsOpenForRegistration(ActionKey)) {
        return;
    }

    auto &Registry = UE::Ranges::FVariantObjectStructRegistry::Get();
    // clang-format off
    for (auto AllData = Registry.GetAllRegisteredStructs() |
                        UE::Ranges::Filter([](const UE::Ranges::FRegisteredVariantStruct &StructType) {
                            return UEdGraphSchema_K2::IsAllowableBlueprintVariableType(
                                StructType.GetStructType(), true);
                        }) |
                        UE::Ranges::Map(&UE::Ranges::FRegisteredVariantStruct::GetClassesWithStructType) |
                        UE::Ranges::Join;
         auto [Struct, Class] : AllData) {
        auto Spawner = UBlueprintNodeSpawner::Create(ActionKey);
        check(Spawner)

        Spawner->CustomizeNodeDelegate =
            UBlueprintNodeSpawner::FCustomizeNodeDelegate::CreateStatic(CustomizeCallback, Class, Struct);
        ActionRegistrar.AddBlueprintAction(ActionKey, Spawner);
    }
    // clang-format on
}

void UK2Node_MakeVariantObjectStruct::ExpandNode(FKismetCompilerContext &CompilerContext, UEdGraph *SourceGraph) {
    Super::ExpandNode(CompilerContext, SourceGraph);
}