// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "BagMenuHandler.h"
#include "GiveItemHandler.generated.h"

/**
 * The handler used to manage giving an item to a Pokémon from the bag
 */
UCLASS(EditInlineNew)
class POKEMONUI_API UGiveItemHandler : public UBagMenuHandler {
    GENERATED_BODY()

protected:
    bool ShouldShow_Implementation(const TScriptInterface<IInventoryScreen>& Screen, const FItem &Item, int32 Quantity) const override;
    void Handle_Implementation(const TScriptInterface<IInventoryScreen>& Screen, const FItem &Item, int32 Quantity) override;

};
