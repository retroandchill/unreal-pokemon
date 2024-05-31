// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Bag.h"
#include "ItemSlot.h"
#include "UObject/Object.h"

#include "DefaultBag.generated.h"

/**
 * Wrapper around the items in a pocket in the game.
 */
USTRUCT(BlueprintType)
struct POKEMONCORE_API FPocket {
    GENERATED_BODY()

    /**
     * The items that have been placed inside that pocket.
     */
    UPROPERTY(SaveGame)
    TArray<FItemSlot> Items;
};

/**
 * The default implementation of the player's bag.
 */
UCLASS(Blueprintable)
class POKEMONCORE_API UDefaultBag : public UObject, public IBag {
    GENERATED_BODY()

  public:
    void Initialize() override;

    UFUNCTION(BlueprintPure, Category = "Player|Inventory")
    int32 GetItemQuantity(FName ItemID) const override;

    UFUNCTION(BlueprintCallable, Category = "Player|Inventory")
    int32 ObtainItem(FName ItemID, int32 Amount = 1) override;

    UFUNCTION(BlueprintCallable, Category = "Player|Inventory")
    int32 RemoveItem(FName ItemID, int32 Amount = 1) override;

    void SortPocket(uint8 Pocket, const IBagSorter &Sorter) override;
    void ForEachInPocket(uint8 Pocket, const TFunctionRef<void(FName, int32)>& Callback) const override;

  private:
    /**
     * Get the pocket for the given item ID, creating a new empty one if it doesn't already exist
     * @param ItemID The ID of the item in question
     * @return The pocket the item should go in
     */
    TArray<FItemSlot> &GetPocket(FName ItemID);

    /**
     * Get the pocket for the given item ID
     * @param ItemID The ID of the item in question
     * @return The pocket the item should go in (null if so such pocket exists)
     */
    const TArray<FItemSlot> *GetPocket(FName ItemID) const;

    /**
     * The actual representation of the items in the player's inventory.
     */
    UPROPERTY(SaveGame)
    TMap<uint8, FPocket> ItemSlots;
};
