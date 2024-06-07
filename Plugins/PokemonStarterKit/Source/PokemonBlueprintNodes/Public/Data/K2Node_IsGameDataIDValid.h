// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "K2Node.h"

#include "K2Node_IsGameDataIDValid.generated.h"

/**
 * Custom Blueprint node designed to check if a GameData ID is valid or not
 */
UCLASS()
class POKEMONBLUEPRINTNODES_API UK2Node_IsGameDataIDValid : public UK2Node {
    GENERATED_BODY()

  public:
    /**
     * Set up the node assigning the struct that this should be retrieving
     * @param NodeStruct The struct type for this node
     */
    void Initialize(UScriptStruct *NodeStruct);

    void AllocateDefaultPins() override;
    FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
    FText GetTooltipText() const override;
    bool IsNodePure() const override;
    FText GetMenuCategory() const override;
    FSlateIcon GetIconAndTint(FLinearColor &OutColor) const override;

    void GetMenuActions(FBlueprintActionDatabaseRegistrar &ActionRegistrar) const override;
    void ExpandNode(class FKismetCompilerContext &CompilerContext, UEdGraph *SourceGraph) override;

  private:
    /**
     * The struct type represented by this node
     */
    UPROPERTY()
    TObjectPtr<UScriptStruct> StructType;
};
