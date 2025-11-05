// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "K2Node.h"
#include "K2Node_VariantCastBase.h"

#include "K2Node_CastObjectToVariant.generated.h"

/**
 * Node for handling the cast of an object to a variant.
 */
UCLASS()
class RETROLIBBLUEPRINTNODES_API UK2Node_CastObjectToVariant : public UK2Node_VariantCastBase {
    GENERATED_BODY()

  public:
    /**
     * Set up the node assigning the struct that this should be retrieving
     * @param Output The type of the output node
     */
    void Initialize(UScriptStruct *Output);

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
    TObjectPtr<UScriptStruct> OutputType;
};
