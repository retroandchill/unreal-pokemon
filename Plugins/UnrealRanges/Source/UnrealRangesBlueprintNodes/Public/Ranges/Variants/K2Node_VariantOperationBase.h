// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "K2Node.h"
#include "Ranges/Variants/VariantObjectStruct.h"

#include "K2Node_VariantOperationBase.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class UNREALRANGESBLUEPRINTNODES_API UK2Node_VariantOperationBase : public UK2Node {
    GENERATED_BODY()

public:
    FText GetMenuCategory() const override;
    bool IsNodePure() const override;
    FSlateIcon GetIconAndTint(FLinearColor &OutColor) const override;
    void GetMenuActions(FBlueprintActionDatabaseRegistrar &ActionRegistrar) const override;

protected:
    virtual void AddMenuOptionsForStruct(FBlueprintActionDatabaseRegistrar &ActionRegistrar,
                                         UE::Ranges::IVariantRegistration& Registration) const PURE_VIRTUAL(AddMenuOptionsForStruct);

};
