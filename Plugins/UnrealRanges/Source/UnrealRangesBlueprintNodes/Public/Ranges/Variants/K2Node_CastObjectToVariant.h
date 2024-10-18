// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "K2Node.h"
#include "K2Node_CastObjectToVariant.generated.h"

/**
 * Node for handling the cast of an object to a variant.
 */
UCLASS()
class UNREALRANGESBLUEPRINTNODES_API UK2Node_CastObjectToVariant : public UK2Node {
    GENERATED_BODY()

public:
    /**
     * Set up the node assigning the struct that this should be retrieving
     * @param Output The type of the output node
     */
    void Initialize(UScriptStruct *Output);

    void AllocateDefaultPins() override;
    FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
    FText GetTooltipText() const override;
    bool IsNodePure() const override;
    FText GetMenuCategory() const override;
    FLinearColor GetNodeTitleColor() const override;
    FSlateIcon GetIconAndTint(FLinearColor &OutColor) const override;

    void GetMenuActions(FBlueprintActionDatabaseRegistrar &ActionRegistrar) const override;
    void ExpandNode(FKismetCompilerContext &CompilerContext, UEdGraph *SourceGraph) override;

private:
    UPROPERTY()
    TObjectPtr<UScriptStruct> OutputType;
    
};
