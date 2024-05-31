// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "Bag.generated.h"

class IBagSorter;
// This class does not need to be modified.
UINTERFACE(BlueprintType, NotBlueprintable, meta = (Injectable))
class UBag : public UInterface {
    GENERATED_BODY()
};

/**
 * Declaration of the player's bag.
 */
class POKEMONCORE_API IBag {
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
  public:
    virtual void Initialize() = 0;

    /**
     * Get the count of a specific item.
     * @param ItemID The internal ID of the item
     * @return The number the player has (0 means the player doesn't have any)
     */
    UFUNCTION(BlueprintCallable, Category = "Player|Inventory")
    virtual int32 GetItemQuantity(FName ItemID) const = 0;

    /**
     * Add an item to the player's inventory.
     * @param ItemID The internal ID of the item
     * @param Amount The quantity of items to add
     * @return The number of items that were actually obtained.
     */
    UFUNCTION(BlueprintCallable, Category = "Player|Inventory")
    virtual int32 ObtainItem(FName ItemID, int32 Amount = 1) = 0;

    /**
     * Add an item to the player's inventory.
     * @param ItemID The internal ID of the item
     * @param Amount The quantity of items to add
     * @return The number of items that were actually removed.
     */
    UFUNCTION(BlueprintCallable, Category = "Player|Inventory")
    virtual int32 RemoveItem(FName ItemID, int32 Amount = 1) = 0;

    /**
     * Sort the specified pocket with the provided sorter
     * @param Pocket The pocket to sort
     * @param Sorter The sorter to use
     */
    virtual void SortPocket(uint8 Pocket, const IBagSorter &Sorter) = 0;

    /**
     * Iterate over each item in the given pocket
     * @param Pocket The pocket in question
     * @param Callback The callback for each iteration of the loop
     */
    virtual void ForEachInPocket(uint8 Pocket, const TFunctionRef<void(FName, int32)>& Callback) const = 0;

    /**
     * Transform the elements in the provided pocket to a different data type
     * @tparam T The type of the resultant array
     * @param Pocket The pocket to map
     * @param Mapping The function used to perform the mapping
     * @return The mapped results
     */
    template <typename T>
    TArray<T> TransformPocket(uint8 Pocket, TFunctionRef<T(FName, int32)> Mapping) const {
        TArray<T> Output;
        ForEachInPocket(Pocket,
                        [&Output, &Mapping](FName Item, int32 Quantity) { Output.Add(Mapping(Item, Quantity)); });
        return Output;
    }
};