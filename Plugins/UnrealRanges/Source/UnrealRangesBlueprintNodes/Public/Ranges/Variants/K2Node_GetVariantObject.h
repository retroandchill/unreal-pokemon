// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "K2Node_VariantOperationBase.h"
#include "K2Node_GetVariantObject.generated.h"

/**
 * Get the underlying object of a variant struct.
 */
UCLASS()
class UNREALRANGESBLUEPRINTNODES_API UK2Node_GetVariantObject : public UK2Node_VariantOperationBase {
    GENERATED_BODY()

public:
    /**
     * Set up the node assigning the struct that this should be retrieving
     * @param Input The type of the input node
     */
    void Initialize(UScriptStruct *Input);

    void AllocateDefaultPins() override;
    FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
    FText GetCompactNodeTitle() const override;
    bool ShouldDrawCompact() const override;
    FText GetTooltipText() const override;
    void ExpandNode(FKismetCompilerContext &CompilerContext, UEdGraph *SourceGraph) override;

protected:
    void AddMenuOptionsForStruct(FBlueprintActionDatabaseRegistrar &ActionRegistrar,
        UE::Ranges::IVariantRegistration& Registration) const override;

private:
    UPROPERTY()
    TObjectPtr<UScriptStruct> InputType;
};
