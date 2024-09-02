// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "BagDTO.h"
#include "UObject/Interface.h"

#include "Bag.generated.h"

class IBagSorter;

// This class does not need to be modified.
UINTERFACE(NotBlueprintable, BlueprintType, meta = (Injectable))
class POKEMONCORE_API UBag : public UInterface {
    GENERATED_BODY()
};

/**
 * Declaration of the player's bag.
 */
class POKEMONCORE_API IBag {
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
  public:
    virtual void Initialize(const FBagDTO &DTO = FBagDTO()) = 0;

    virtual FBagDTO ToDTO() const = 0;

    /**
     * Get the count of a specific item.
     * @param ItemID The internal ID of the item
     * @return The number the player has (0 means the player doesn't have any)
     */
    UFUNCTION(BlueprintCallable, Category = "Player|Inventory")
    virtual int32 GetItemQuantity(FItemHandle ItemID) const = 0;

    UFUNCTION(BlueprintCallable, Category = "Player|Inventory")
    virtual bool HasItemWithTag(FName Tag) const = 0;

    /**
     * Is the player able to obtain an item of this type.
     * @param ItemID The ID of the item to check against
     * @return Can the item be added.
     */
    UFUNCTION(BlueprintCallable, Category = "Player|Inventory")
    virtual bool CanObtainItem(FItemHandle ItemID) const = 0;

    /**
     * Add an item to the player's inventory.
     * @param ItemID The internal ID of the item
     * @param Amount The quantity of items to add
     * @return The number of items that were actually obtained.
     */
    UFUNCTION(BlueprintCallable, Category = "Player|Inventory")
    virtual int32 ObtainItem(FItemHandle ItemID, int32 Amount = 1) = 0;

    /**
     * Add an item to the player's inventory.
     * @param ItemID The internal ID of the item
     * @param Amount The quantity of items to add
     * @return The number of items that were actually removed.
     */
    UFUNCTION(BlueprintCallable, Category = "Player|Inventory")
    virtual int32 RemoveItem(FItemHandle ItemID, int32 Amount = 1) = 0;

    /**
     * Sort the specified pocket with the provided sorter
     * @param Pocket The pocket to sort
     * @param Sorter The sorter to use
     */
    virtual void SortPocket(FName Pocket, const IBagSorter &Sorter) = 0;

    /**
     * Iterate over each item in the given pocket
     * @param Pocket The pocket in question
     * @param Callback The callback for each iteration of the loop
     */
    virtual void ForEachInPocket(FName Pocket, const TFunctionRef<void(FName, int32)> &Callback) const = 0;

    /**
     * Get a view of all the items in the player's inventory.
     * @return The view of all items in the inventory
     */
    virtual void ForEachItem(const TFunctionRef<void(FName, int32)> &Callback) const = 0;
};