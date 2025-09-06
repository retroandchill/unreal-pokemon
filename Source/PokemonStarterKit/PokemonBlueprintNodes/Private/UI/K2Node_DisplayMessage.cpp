// "Unreal Pokémon" created by Retro & Chill.
#include "UI/K2Node_DisplayMessage.h"
#include "EdGraphSchema_K2.h"
#include "K2Node_AsyncAction.h"
#include "Nodes/DisplayMessage.h"

class UK2Node_AsyncAction;

UK2Node_DisplayMessage::UK2Node_DisplayMessage(const FObjectInitializer &ObjectInitializer) : Super(ObjectInitializer)
{
}

void UK2Node_DisplayMessage::GetMenuActions(FBlueprintActionDatabaseRegistrar &ActionRegistrar) const
{
    auto FactoryFunc = UDisplayMessage::StaticClass()->FindFunctionByName("DisplayMessage");
    check(FactoryFunc != nullptr)
    SupplyMenuActions(ActionRegistrar, FactoryFunc);
}

void UK2Node_DisplayMessage::ExpandNode(FKismetCompilerContext &CompilerContext, UEdGraph *SourceGraph)
{
    static const FName OnConfirmPinName(TEXT("OnConfirm"));

    auto OnConfirmPin = FindPinChecked(OnConfirmPinName);
    ReconnectOutputPin(CompilerContext, OnConfirmPin);

    Super::ExpandNode(CompilerContext, SourceGraph);
}
