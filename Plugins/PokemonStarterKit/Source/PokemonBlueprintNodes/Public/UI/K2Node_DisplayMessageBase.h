// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "K2Node_ScreenBoundNode.h"
#include "MessageNode.h"

#include "K2Node_DisplayMessageBase.generated.h"

class UTextDisplayScreen;
/**
 * Basic template of any async node that requires the display of a message
 */
UCLASS(Abstract)
class POKEMONBLUEPRINTNODES_API UK2Node_DisplayMessageBase : public UK2Node_ScreenBoundNode, public IMessageNode {
    GENERATED_BODY()

  protected:
    void ForEachValidScreen(const TFunctionRef<void(UClass *)> &Action) const final;

    /**
     * Check if the supplied pin is connected to another message related node, and if not, add a node to close the
     * window
     * @param CompilerContext The context to compile in
     * @param OutputPin The pin to alter the connections to
     */
    void ReconnectOutputPin(FKismetCompilerContext &CompilerContext, UEdGraphPin *OutputPin);
};
