// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "K2Node_VariantOperationBase.h"

#include "K2Node_MakeSoftVariantFromSoftObject.generated.h"

/**
 * Create a soft variant reference from a valid soft object reference.
 */
UCLASS()
class RETROLIBBLUEPRINTNODES_API UK2Node_MakeSoftVariantFromSoftObject : public UK2Node_VariantOperationBase {
    GENERATED_BODY()

  public:
    void Initialize(UScriptStruct *SoftReference);

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
    TObjectPtr<UScriptStruct> SoftReferenceType;
};
