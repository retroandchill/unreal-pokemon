// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "K2Node_AddPinInterface.h"
#include "K2Node_BaseAsyncTask.h"
#include "K2Node_DisplayMessageBase.h"

#include "K2Node_DisplayMessageWithChoices.generated.h"

/**
 * Blueprint node for handling the display of a message to the player with optional choices
 */
UCLASS()
class POKEMONBLUEPRINTNODES_API UK2Node_DisplayMessageWithChoices : public UK2Node_DisplayMessageBase,
                                                                    public IK2Node_AddPinInterface
{
    GENERATED_BODY()

  public:
    /**
     * Default construct the class using the given initializer
     * @param ObjectInitializer The Unreal provided initializer
     */
    explicit UK2Node_DisplayMessageWithChoices(const FObjectInitializer &ObjectInitializer);

    void AllocateDefaultPins() override;
    void GetNodeContextMenuActions(UToolMenu *Menu, UGraphNodeContextMenuContext *Context) const override;
    void GetMenuActions(FBlueprintActionDatabaseRegistrar &ActionRegistrar) const override;
    void ExpandNode(class FKismetCompilerContext &CompilerContext, UEdGraph *SourceGraph) override;

    void AddInputPin() override;
    void RemoveInputPin(UEdGraphPin *Pin) override;
    bool CanRemovePin(const UEdGraphPin *Pin) const override;

  private:
    /**
     * Add an input and output pin corresponding to the given index
     * @param Index The index to set the pin for
     */
    void AddInputAndOutputPin(int Index);

    /**
     * The count of how many choices are visible
     */
    UPROPERTY()
    int32 ChoiceCount = 0;
};
