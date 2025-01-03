// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "K2Node_VariantOperationBase.h"

#include "K2Node_LoadVariantSynchronous.generated.h"

/**
 *
 */
UCLASS()
class RETROLIBBLUEPRINTNODES_API UK2Node_LoadVariantSynchronous : public UK2Node_VariantOperationBase {
    GENERATED_BODY()

  public:
    void AllocateDefaultPins() override;
    void PostReconstructNode() override;
    bool IsConnectionDisallowed(const UEdGraphPin *MyPin, const UEdGraphPin *OtherPin,
                                FString &OutReason) const override;
    void NotifyPinConnectionListChanged(UEdGraphPin *Pin) override;
    FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
    FText GetTooltipText() const override;
    bool IsNodePure() const override;
    void GetMenuActions(FBlueprintActionDatabaseRegistrar &ActionRegistrar) const override;
    void EarlyValidation(FCompilerResultsLog &MessageLog) const override;
    void ExpandNode(FKismetCompilerContext &CompilerContext, UEdGraph *SourceGraph) override;

  protected:
    UEdGraphPin *GetSoftReferencePin() const;
    UEdGraphPin *GetResultPin() const;
    TOptional<UScriptStruct &> GetInputStruct() const;
    TOptional<UScriptStruct &> GetOutputStruct() const;

  private:
    void RefreshInputPin() const;
};
