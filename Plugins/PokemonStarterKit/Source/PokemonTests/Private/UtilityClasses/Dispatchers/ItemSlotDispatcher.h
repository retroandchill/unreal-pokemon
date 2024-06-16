// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

#include "ItemSlotDispatcher.generated.h"

struct FItem;
/**
 * Dispatcher for an item slot method
 */
UCLASS()
class POKEMONTESTS_API UItemSlotDispatcher : public UObject {
    GENERATED_BODY()

  public:
    UFUNCTION()
    void ReceiveItem(const FItem &Item, int32 ItemQuantity);

    FName ItemID;
    int32 Quantity;
};
