// "Unreal Pokémon" created by Retro & Chill.

#pragma once


#include "CoreMinimal.h"
#include "K2Node.h"
#include "RetroLib/RetroLibMacros.h"
#include <RetroLib/Utils/Unreachable.h>

#include "K2Node_VariantOperationBase.generated.h"

namespace Retro {
  class IVariantRegistration;
}

/**
 *
 */
UCLASS(Abstract)
class RETROLIBBLUEPRINTNODES_API UK2Node_VariantOperationBase : public UK2Node {
    GENERATED_BODY()

  public:
    FText GetMenuCategory() const override;
    bool IsNodePure() const override;
    FSlateIcon GetIconAndTint(FLinearColor &OutColor) const override;
    void GetMenuActions(FBlueprintActionDatabaseRegistrar &ActionRegistrar) const override;

  protected:
    virtual void AddMenuOptionsForStruct(FBlueprintActionDatabaseRegistrar &ActionRegistrar,
                                         Retro::IVariantRegistration &Registration) const ABSTRACT_METHOD;
};
