// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Bag.generated.h"

// This class does not need to be modified.
UINTERFACE(BlueprintType, NotBlueprintable)
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
    /**
     * Get the count of a specific item.
     * @param ItemID The internal ID of the item
     * @return The number the player has (0 means the player doesn't have any)
     */
    UFUNCTION(BlueprintPure, Category = "Player|Inventory")
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

};