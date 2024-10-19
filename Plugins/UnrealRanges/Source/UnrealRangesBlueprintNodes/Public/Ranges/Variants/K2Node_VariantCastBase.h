// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "K2Node.h"
#include "K2Node_VariantOperationBase.h"

#include "K2Node_VariantCastBase.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class UNREALRANGESBLUEPRINTNODES_API UK2Node_VariantCastBase : public UK2Node_VariantOperationBase {
    GENERATED_BODY()

protected:
    struct FCastFunctionInfo {
        UK2Node* CastNode = nullptr;
        UEdGraphPin* InputPin = nullptr;
        UEdGraphPin* OutputPin = nullptr;
    };

public:
    void AllocateDefaultPins() override;
    FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
    FText GetTooltipText() const override;
    bool IsNodePure() const override;
    FLinearColor GetNodeTitleColor() const override;
    FSlateIcon GetIconAndTint(FLinearColor &OutColor) const override;

    void GetNodeContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const override;
    void GetMenuActions(FBlueprintActionDatabaseRegistrar &ActionRegistrar) const override;
    void ExpandNode(FKismetCompilerContext &CompilerContext, UEdGraph *SourceGraph) override;

protected:
    virtual void CreateInputAndOutputPins() PURE_VIRTUAL(CreateInputAndOutputPins);
    UEdGraphPin* GetCastFailedPin() const;

    virtual void MakeAdditionalPinLinks(UK2Node& CallPerformCast) const;
    virtual UEdGraphPin* GetInputPin() const PURE_VIRTUAL(GetInputPin, return nullptr;);
    virtual UEdGraphPin* GetOutputPin() const PURE_VIRTUAL(GetOutputPin, return nullptr;);
    UEdGraphPin* GetCastSucceededPin() const;
    
    virtual FCastFunctionInfo GetPerformCastNode(FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph) PURE_VIRTUAL(GetPerformCastNode, return FCastFunctionInfo(););

private:
    void TogglePurity();
    void SetPurity(bool bPurity);
    
    UPROPERTY()
    bool bIsPure = false;
};
