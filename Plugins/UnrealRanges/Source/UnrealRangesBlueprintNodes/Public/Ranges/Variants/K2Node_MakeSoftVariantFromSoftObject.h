// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "K2Node_VariantOperationBase.h"
#include "K2Node_MakeSoftVariantFromSoftObject.generated.h"

/**
 * Create a soft variant reference from a valid soft object reference.
 */
UCLASS()
class UNREALRANGESBLUEPRINTNODES_API UK2Node_MakeSoftVariantFromSoftObject : public UK2Node_VariantOperationBase {
    GENERATED_BODY()

public:
    void Initialize(UClass* Object, UScriptStruct *SoftReference);

    void AllocateDefaultPins() override;
    FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
    FText GetTooltipText() const override;
    void ExpandNode(FKismetCompilerContext &CompilerContext, UEdGraph *SourceGraph) override;

protected:
    void AddMenuOptionsForStruct(FBlueprintActionDatabaseRegistrar &ActionRegistrar,
        UE::Ranges::IVariantRegistration& Registration) const override;

private:
    UPROPERTY()
    TObjectPtr<UClass> ObjectType;
    
    UPROPERTY()
    TObjectPtr<UScriptStruct> SoftReferenceType;

};
