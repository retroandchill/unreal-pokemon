// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "K2Node_VariantOperationBase.h"

#include "K2Node_MakeSoftVariantFromVariant.generated.h"


/**
 * Make a soft variant struct from its corresponding hard reference.
 */
UCLASS()
class RETROLIBBLUEPRINTNODES_API UK2Node_MakeSoftVariantFromVariant : public UK2Node_VariantOperationBase {
    GENERATED_BODY()

  public:
    void Initialize(UScriptStruct *HardReference, UScriptStruct *SoftReference);

    void AllocateDefaultPins() override;
    FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
    FText GetTooltipText() const override;
    void ExpandNode(FKismetCompilerContext &CompilerContext, UEdGraph *SourceGraph) override;

  protected:
    void AddMenuOptionsForStruct(FBlueprintActionDatabaseRegistrar &ActionRegistrar,
                                 Retro::IVariantRegistration &Registration) const override;

  private:
    UPROPERTY()
    TObjectPtr<UScriptStruct> HardReferenceType;

    UPROPERTY()
    TObjectPtr<UScriptStruct> SoftReferenceType;
};
