// "Unreal Pokémon" created by Retro & Chill.
#include "UI/K2Node_DisplayMessageBase.h"
#include "K2Node_CallFunction.h"
#include "KismetCompiler.h"
#include "Screens/TextDisplayScreen.h"
#include "Utilities/RPGMenuUtilities.h"

void UK2Node_DisplayMessageBase::ForEachValidScreen(const TFunctionRef<void(UClass *)> &Action) const {
    for (TObjectIterator<UClass> It; It; ++It) {
        if (!It->IsChildOf(UTextDisplayScreen::StaticClass()) || It->HasAnyClassFlags(CLASS_Abstract)) {
            continue;
        }
        Action(*It);
    }
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
