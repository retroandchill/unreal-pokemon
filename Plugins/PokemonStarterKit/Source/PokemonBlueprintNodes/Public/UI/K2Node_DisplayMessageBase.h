// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "K2Node_BaseAsyncTask.h"
#include "MessageNode.h"

#include "K2Node_DisplayMessageBase.generated.h"

class UTextDisplayScreen;
/**
 * Basic template of any async node that requires the display of a message
 */
UCLASS(Abstract)
class POKEMONBLUEPRINTNODES_API UK2Node_DisplayMessageBase : public UK2Node_BaseAsyncTask, public IMessageNode {
    GENERATED_BODY()

public:
    explicit UK2Node_DisplayMessageBase(const FObjectInitializer &Initializer);
    
  protected:
    /**
     * Check if the supplied pin is connected to another message related node, and if not, add a node to close the
     * window
     * @param CompilerContext The context to compile in
     * @param OutputPin The pin to alter the connections to
     */
    void ReconnectOutputPin(FKismetCompilerContext &CompilerContext, UEdGraphPin *OutputPin);

    /**
     * Add the supplied menu actions for each Display Text Screen subclass
     * @param ActionRegistrar Used to register nodes
     * @param FactoryFunc The function used to actually produce the nodes
     */
    void SupplyMenuActions(FBlueprintActionDatabaseRegistrar &ActionRegistrar, UFunction *FactoryFunc) const;
};
