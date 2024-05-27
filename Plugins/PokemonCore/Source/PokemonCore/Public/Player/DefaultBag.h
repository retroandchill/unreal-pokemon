// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Bag.h"
#include "UObject/Object.h"
#include "DefaultBag.generated.h"

struct FItemSlot;

/**
 * The default implementation of the player's bag.
 */
UCLASS(Blueprintable)
class POKEMONCORE_API UDefaultBag : public UObject, public IBag {
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintPure, Category = "Player|Inventory")
    int32 GetItemQuantity(FName ItemID) const override;
    
    UFUNCTION(BlueprintCallable, Category = "Player|Inventory")
    int32 ObtainItem(FName ItemID, int32 Amount) override;
    
    UFUNCTION(BlueprintCallable, Category = "Player|Inventory")
    int32 RemoveItem(FName ItemID, int32 Amount) override;

private:
    /**
     * The actual representation of the items in the player's inventory.
     */
    UPROPERTY()
    TArray<FItemSlot> ItemSlots;

};
