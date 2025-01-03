// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "K2Node.h"
#include "K2Node_VariantOperationBase.h"

#include "K2Node_MakeVariantObjectStruct.generated.h"

/**
 * K2 Node to make a variant object struct from a valid object type.
 */
UCLASS()
class RETROLIBBLUEPRINTNODES_API UK2Node_MakeVariantObjectStruct : public UK2Node_VariantOperationBase {
    GENERATED_BODY()

  public:
    /**
     * Set up the node assigning the struct that this should be retrieving
     * @param Output The type of the output node
     */
    void Initialize(UScriptStruct *Output);

    void AllocateDefaultPins() override;
    void PostReconstructNode() override;
    bool IsConnectionDisallowed(const UEdGraphPin *MyPin, const UEdGraphPin *OtherPin,
                                FString &OutReason) const override;
    void NotifyPinConnectionListChanged(UEdGraphPin *Pin) override;
    FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
    FText GetTooltipText() const override;
    void EarlyValidation(FCompilerResultsLog &MessageLog) const override;
    void ExpandNode(FKismetCompilerContext &CompilerContext, UEdGraph *SourceGraph) override;

  protected:
    void AddMenuOptionsForStruct(FBlueprintActionDatabaseRegistrar &ActionRegistrar,
                                 Retro::IVariantRegistration &Registration) const override;

    UEdGraphPin *GetObjectPin() const;
    TOptional<UClass &> GetInputClass() const;

  private:
    void RefreshInputPinType() const;

    UPROPERTY()
    TObjectPtr<UScriptStruct> OutputType;
};
