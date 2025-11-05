// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "K2Node.h"
#include "K2Node_VariantCastBase.h"

#include "K2Node_GetVariantValue.generated.h"

/**
 * Get the value of a variant object, with a check for an invalid value
 */
UCLASS()
class RETROLIBBLUEPRINTNODES_API UK2Node_GetVariantValue : public UK2Node_VariantCastBase {
    GENERATED_BODY()

  public:
    /**
     * Set up the node assigning the struct that this should be retrieving
     * @param Input The type of the input node
     * @param Output The type of the output node
     */
    void Initialize(UScriptStruct *Input, UClass *Output);

    FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
    FText GetTooltipText() const override;

  protected:
    void CreateInputAndOutputPins() override;
    void AddMenuOptionsForStruct(FBlueprintActionDatabaseRegistrar &ActionRegistrar,
                                 Retro::IVariantRegistration &Registration) const override;
    UEdGraphPin *GetInputPin() const override;
    UEdGraphPin *GetOutputPin() const override;
    FCastFunctionInfo GetPerformCastNode(FKismetCompilerContext &CompilerContext, UEdGraph *SourceGraph) override;
    void MakeAdditionalPinLinks(UK2Node &CallPerformCast) const override;

  private:
    UPROPERTY()
    TObjectPtr<UScriptStruct> InputType;

    UPROPERTY()
    TObjectPtr<UClass> OutputType;
};
