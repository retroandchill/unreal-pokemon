// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "K2Node_VariantOperationBase.h"

#include "K2Node_GetVariantObject.generated.h"

/**
 * Get the underlying object of a variant struct.
 */
UCLASS()
class RETROLIBBLUEPRINTNODES_API UK2Node_GetVariantObject : public UK2Node_VariantOperationBase {
    GENERATED_BODY()

  public:
    void AllocateDefaultPins() override;
    void PostReconstructNode() override;
    bool IsConnectionDisallowed(const UEdGraphPin *MyPin, const UEdGraphPin *OtherPin,
                                FString &OutReason) const override;
    void NotifyPinConnectionListChanged(UEdGraphPin *Pin) override;
    FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
    FText GetCompactNodeTitle() const override;
    bool ShouldDrawCompact() const override;
    FText GetTooltipText() const override;
    void GetMenuActions(FBlueprintActionDatabaseRegistrar &ActionRegistrar) const override;
    void EarlyValidation(FCompilerResultsLog &MessageLog) const override;
    void ExpandNode(FKismetCompilerContext &CompilerContext, UEdGraph *SourceGraph) override;

  protected:
    UEdGraphPin *GetVariantPin() const;
    TOptional<UScriptStruct &> GetInputStruct() const;

  private:
    void RefreshInputPin() const;
};
