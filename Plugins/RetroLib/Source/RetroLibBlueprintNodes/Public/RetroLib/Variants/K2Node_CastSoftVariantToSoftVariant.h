// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "K2Node_VariantCastBase.h"

#include "K2Node_CastSoftVariantToSoftVariant.generated.h"

/**
 *
 */
UCLASS()
class RETROLIBBLUEPRINTNODES_API UK2Node_CastSoftVariantToSoftVariant : public UK2Node_VariantCastBase {
    GENERATED_BODY()

  public:
    void Initialize(UScriptStruct *Input, UScriptStruct *Output);

    FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
    FText GetTooltipText() const override;

  protected:
    void CreateInputAndOutputPins() override;
    void AddMenuOptionsForStruct(FBlueprintActionDatabaseRegistrar &ActionRegistrar,
                                 Retro::IVariantRegistration &Registration) const override;
    UEdGraphPin *GetInputPin() const override;
    UEdGraphPin *GetOutputPin() const override;
    FCastFunctionInfo GetPerformCastNode(FKismetCompilerContext &CompilerContext, UEdGraph *SourceGraph) override;

  private:
    UPROPERTY()
    TObjectPtr<UScriptStruct> InputType;

    UPROPERTY()
    TObjectPtr<UScriptStruct> OutputType;
};
