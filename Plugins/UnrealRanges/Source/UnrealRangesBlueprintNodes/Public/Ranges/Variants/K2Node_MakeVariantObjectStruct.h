// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "K2Node.h"
#include "K2Node_MakeVariantObjectStruct.generated.h"

/**
 * K2 Node to make a variant object struct from a valid object type.
 */
UCLASS()
class UNREALRANGESBLUEPRINTNODES_API UK2Node_MakeVariantObjectStruct : public UK2Node {
    GENERATED_BODY()

public:
    /**
     * Set up the node assigning the struct that this should be retrieving
     * @param Input The type of the input node
     * @param Output The type of the output node
     */
    void Initialize(UClass *Input, UScriptStruct *Output);

    void AllocateDefaultPins() override;
    FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
    FText GetTooltipText() const override;
    bool IsNodePure() const override;
    FText GetMenuCategory() const override;
    FSlateIcon GetIconAndTint(FLinearColor &OutColor) const override;

    void GetMenuActions(FBlueprintActionDatabaseRegistrar &ActionRegistrar) const override;
    void ExpandNode(FKismetCompilerContext &CompilerContext, UEdGraph *SourceGraph) override;

private:
    UPROPERTY()
    TObjectPtr<UClass> InputType;
    
    UPROPERTY()
    TObjectPtr<UScriptStruct> OutputType;

};
