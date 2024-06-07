// "Unreal Pokémon" created by Retro & Chill.

#include "UI/K2Node_ScreenBoundNode.h"
#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintNodeSpawner.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Screens/Screen.h"

UK2Node_ScreenBoundNode::UK2Node_ScreenBoundNode(const FObjectInitializer &Initializer) : Super(Initializer) {
    ProxyActivateFunctionName = GET_FUNCTION_NAME_CHECKED(UBlueprintAsyncActionBase, Activate);
}

void UK2Node_ScreenBoundNode::Initialize(TSubclassOf<UScreen> NodeClass, TSharedRef<uint32> NodeCounter) {
    ScreenType = NodeClass;
    TotalScreens = MoveTemp(NodeCounter);
}

void UK2Node_ScreenBoundNode::AllocateDefaultPins() {
    Super::AllocateDefaultPins();
    static const FName ScreenType_ParamName(TEXT("ScreenClass"));
    auto ScreenClassPin = FindPinChecked(ScreenType_ParamName);
    ScreenClassPin->DefaultObject = ScreenType;
    ScreenClassPin->bHidden = true;
}

FText UK2Node_ScreenBoundNode::GetNodeTitle(ENodeTitleType::Type TitleType) const {
    if (ScreenType != nullptr && *TotalScreens > 1) {
        return FText::FormatNamed(
            NSLOCTEXT("K2Node", "NodeScreenBoundNode_NodeTitleFormat", "{OriginalName} ({ClassName})"),
            TEXT("OriginalName"), Super::GetNodeTitle(TitleType), TEXT("ClassName"), ScreenType->GetDisplayNameText());
    }

    return Super::GetNodeTitle(TitleType);
}

void UK2Node_ScreenBoundNode::SupplyMenuActions(FBlueprintActionDatabaseRegistrar &ActionRegistrar,
                                                UFunction *FactoryFunc) const {
    auto CustomizeCallback = [](UEdGraphNode *Node, [[maybe_unused]] bool bIsTemplateNode,
                                TSubclassOf<UScreen> Subclass, TSharedRef<uint32> NodeCounter, UFunction *Factory) {
        auto TypedNode = CastChecked<UK2Node_ScreenBoundNode>(Node);
        auto ReturnProp = CastFieldChecked<FObjectProperty>(Factory->GetReturnProperty());

        TypedNode->ProxyFactoryFunctionName = Factory->GetFName();
        TypedNode->ProxyFactoryClass = Factory->GetOuterUClass();
        TypedNode->ProxyClass = ReturnProp->PropertyClass;
        TypedNode->Initialize(Subclass, MoveTemp(NodeCounter));
    };

    auto ScreenCounter = MakeShared<uint32>(0);
    ForEachValidScreen([this, &ScreenCounter, &CustomizeCallback, &FactoryFunc, &ActionRegistrar](UClass *MenuClass) {
        if (!UEdGraphSchema_K2::IsAllowableBlueprintVariableType(MenuClass, true)) {
            return;
        }

        auto Spawner = UBlueprintNodeSpawner::Create(GetClass());
        check(Spawner != nullptr)

        (*ScreenCounter)++;
        Spawner->CustomizeNodeDelegate = UBlueprintNodeSpawner::FCustomizeNodeDelegate::CreateStatic(
            CustomizeCallback, TSubclassOf<UScreen>(MenuClass), ScreenCounter, FactoryFunc);
        ActionRegistrar.AddBlueprintAction(GetClass(), Spawner);
    });
}

void UK2Node_ScreenBoundNode::ForEachValidScreen(const TFunctionRef<void(UClass *)> &Action) const {
    // No action by default
}