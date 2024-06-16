// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Handlers/BagMenu/BagMenuHandler.h"

#include "SampleHandler.generated.h"

/**
 *
 */
UCLASS()
class POKEMONTESTS_API USampleHandler : public UBagMenuHandler {
    GENERATED_BODY()

  protected:
    void Handle_Implementation(const TScriptInterface<IInventoryScreen> &Screen, const FItem &Item,
                               int32 Quantity) override;

  public:
    FName ItemID;
    int32 ItemQuantity;
};
