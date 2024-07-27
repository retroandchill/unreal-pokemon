// "Unreal Pokémon" created by Retro & Chill.
#include "UI/K2Node_DisplayMessageBase.h"
#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintNodeSpawner.h"
#include "K2Node_CallFunction.h"
#include "KismetCompiler.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Screens/TextDisplayScreen.h"
#include "Utilities/RPGMenuUtilities.h"

UK2Node_DisplayMessageBase::UK2Node_DisplayMessageBase(const FObjectInitializer &Initializer) : Super(Initializer) {
    ProxyActivateFunctionName = GET_FUNCTION_NAME_CHECKED(UBlueprintAsyncActionBase, Activate);
}

void UK2Node_DisplayMessageBase::ReconnectOutputPin(FKismetCompilerContext &CompilerContext, UEdGraphPin *OutputPin) {
    if (OutputPin->LinkedTo.ContainsByPredicate([](const UEdGraphPin *const Link) {
            return Link->GetOwningNode()->GetClass()->ImplementsInterface(UMessageNode::StaticClass());
        }))
        return;

    const FName FunctionName =
        GET_FUNCTION_NAME_CHECKED_OneParam(URPGMenuUtilities, RemoveTopScreenFromOverlay, UObject *);
    auto IntermediateNode = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, GetGraph());
    IntermediateNode->FunctionReference.SetExternalMember(FunctionName, URPGMenuUtilities::StaticClass());
    IntermediateNode->AllocateDefaultPins();

    if (auto This_WorldContextPin = FindPin(TEXT("WorldContextObject")); This_WorldContextPin != nullptr) {
        auto Intermediate_WorldContextPin = IntermediateNode->FindPinChecked(TEXT("WorldContextObject"));
        CompilerContext.CopyPinLinksToIntermediate(*This_WorldContextPin, *Intermediate_WorldContextPin);
    }

    auto InputPin = IntermediateNode->FindPinChecked(UEdGraphSchema_K2::PN_Execute);
    auto ThenPin = IntermediateNode->FindPinChecked(UEdGraphSchema_K2::PN_Then);

    CompilerContext.MovePinLinksToIntermediate(*OutputPin, *ThenPin);
    OutputPin->MakeLinkTo(InputPin);
}

void UK2Node_DisplayMessageBase::SupplyMenuActions(FBlueprintActionDatabaseRegistrar &ActionRegistrar, UFunction *FactoryFunc) const {
    auto CustomizeCallback = [](UEdGraphNode *Node, [[maybe_unused]] bool bIsTemplateNode, UFunction *Factory) {
        auto TypedNode = CastChecked<UK2Node_DisplayMessageBase>(Node);
        auto ReturnProp = CastFieldChecked<FObjectProperty>(Factory->GetReturnProperty());

        TypedNode->ProxyFactoryFunctionName = Factory->GetFName();
        TypedNode->ProxyFactoryClass = Factory->GetOuterUClass();
        TypedNode->ProxyClass = ReturnProp->PropertyClass;
    };

    auto Spawner = UBlueprintNodeSpawner::Create(GetClass());
    check(Spawner != nullptr)

    Spawner->CustomizeNodeDelegate = UBlueprintNodeSpawner::FCustomizeNodeDelegate::CreateLambda(CustomizeCallback, FactoryFunc);
    ActionRegistrar.AddBlueprintAction(GetClass(), Spawner);
}
