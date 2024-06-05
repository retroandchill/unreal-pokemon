// "Unreal Pokémon" created by Retro & Chill.
#include "Nodes/K2Node_DisplayMessageBase.h"
#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintNodeSpawner.h"
#include "K2Node_CallFunction.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "KismetCompiler.h"
#include "PokemonUISettings.h"
#include "RPGMenusSubsystem.h"
#include "Screens/TextDisplayScreen.h"

void UK2Node_DisplayMessageBase::ForEachValidScreen(const TFunctionRef<void(UClass *)> &Action) const {
    auto Settings = GetDefault<UPokemonUISettings>();
    for (auto &MenuClasses = Settings->GetTextScreenClasses(); auto MenuClass : MenuClasses) {
        Action(*MenuClass);
    }
}

void UK2Node_DisplayMessageBase::ReconnectOutputPin(FKismetCompilerContext &CompilerContext, UEdGraphPin *OutputPin) {
    if (OutputPin->LinkedTo.ContainsByPredicate([](const UEdGraphPin *const Link) {
            return Link->GetOwningNode()->GetClass()->ImplementsInterface(UMessageNode::StaticClass());
        }))
        return;

    const FName FunctionName = GET_FUNCTION_NAME_CHECKED_OneParam(URPGMenusSubsystem, RemoveScreenFromStack, UObject *);
    auto IntermediateNode = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, GetGraph());
    IntermediateNode->FunctionReference.SetExternalMember(FunctionName, URPGMenusSubsystem::StaticClass());
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
