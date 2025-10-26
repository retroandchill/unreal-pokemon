// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

#include "InventoryContainer.generated.h"

/**
 * Enum class representing the validation status of an inventory operation.
 *
 * This enum is used to indicate whether an inventory-related operation is valid
 * or invalid.
 */
UENUM(BlueprintType)
enum class EInventoryValidation : uint8
{
    /**
     * The operation or item is deemed valid.
     */
    Valid,

    /**
     * The operation or item is deemed invalid.
     */
    Invalid
};

/**
 * Abstract class representing a container for managing inventory items.
 *
 * This class provides functionality for adding, removing, replacing, and
 * querying items stored in the inventory. It also supports optional
 * limitations on the container's capacity and item stack size.
 */
UCLASS(Abstract, EditInlineNew)
class RPGCORE_API UInventoryContainer : public UObject
{
    GENERATED_BODY()

  public:
    /**
     * Adds a specified amount of an item to the inventory.
     *
     * This method attempts to add the given amount of the specified item to the inventory,
     * ensuring that the item's quantity does not exceed the maximum stack size. If
     * the item cannot be added due to exceeding limits or invalid parameters, the method
     * returns 0.
     *
     * @param ItemID The unique identifier of the item to add.
     * @param Amount The amount of the item to add to the inventory. Must be greater than 0.
     * @return The actual amount of the item that was successfully added to the inventory.
     *         Returns 0 if the item cannot be added.
     */
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    int32 AddItem(FName ItemID, int32 Amount);

    /**
     * Removes a specified amount of an item from the inventory.
     *
     * This method attempts to remove the given amount of the specified item from the inventory.
     * If the item does not exist or the specified amount is invalid, it returns 0. If the amount
     * to remove is greater than the quantity available in the inventory, it removes all available
     * quantity of the item.
     *
     * @param ItemID The unique identifier of the item to remove.
     * @param Amount The amount of the item to remove from the inventory. Must be greater than 0.
     * @return The actual amount of the item that was successfully removed from the inventory.
     *         Returns 0 if the item does not exist or if the amount is invalid.
     */
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    int32 RemoveItem(FName ItemID, int32 Amount);

    /**
     * Replaces an existing item in the inventory with a new item.
     *
     * This method attempts to replace an item identified by `OldItem` with a new item identified
     * by `NewItem`. The replacement is performed only if the following conditions are met:
     * - Both items pass validation.
     * - The inventory contains the item to be replaced (`OldItem`).
     * - The new item (`NewItem`) does not already exist in the inventory.
     * - `OldItem` and `NewItem` are not the same.
     *
     * If replacement is successful, the quantity associated with `OldItem` will be transferred
     * to `NewItem`.
     *
     * @param OldItem The unique identifier of the item to be replaced.
     * @param NewItem The unique identifier of the new item to replace the old item with.
     * @return true if the replacement is successful; false otherwise.
     */
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool ReplaceItem(FName OldItem, FName NewItem);

    /**
     * Clears all items from the inventory container.
     *
     * This method removes all entries from the inventory, effectively
     * resetting the container to an empty state. It does not perform
     * any additional validation checks or trigger any related events.
     */
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void Clear();

    /**
     * Retrieves the quantity of a specified item in the inventory.
     *
     * This method checks the inventory container for the specified item's unique
     * identifier and returns the number of units of that item currently in the inventory.
     * If the item does not exist in the inventory, the returned quantity will be 0.
     *
     * @param ItemID The unique identifier of the item whose quantity is being queried.
     * @return The quantity of the specified item in the inventory. Returns 0 if the item
     *         does not exist.
     */
    UFUNCTION(BlueprintPure, Category = "Inventory")
    int32 GetItemQuantity(FName ItemID) const;

    /**
     * Determines if a specified amount of an item can be added to the inventory.
     *
     * This method checks whether the given amount of an item identified by its unique identifier
     * can be added to the inventory without violating constraints. It ensures that internal
     * and blueprint-based validations are passed before confirming if the item can be added.
     *
     * @param ItemID The unique identifier of the item to check.
     * @param Amount The amount of the item to validate for addition. Must be greater than 0.
     * @return true if the specified amount of the item can be added to the inventory; false otherwise.
     */
    bool CanAddItem(FName ItemID, int32 Amount) const;

    /**
     * Validates the specified item ID to determine if the item is valid for this inventory container.
     *
     * This method performs both native and blueprint-based validation to assess the validity
     * of the item ID provided.
     *
     * @param ItemID The unique identifier of the item to validate.
     * @return True if the item is valid, false otherwise.
     */
    bool ValidateItem(FName ItemID) const;

    /**
     * Retrieves the maximum number of slots available in the inventory.
     *
     * @return An optional integer representing the maximum slots. If not set, the optional will not contain a valid
     * value.
     */
    TOptional<int32> GetMaxSlots() const
    {
        return MaxSlots;
    }

    /**
     * Retrieves the maximum stack size for items in an inventory container.
     *
     * This method returns an optional value representing the maximum number of items
     * that can be stacked together within the inventory container. If the value is not set,
     * the optional object will not contain a valid value.
     *
     * @return An optional integer representing the maximum stack size for items.
     */
    TOptional<int32> GetMaxStackSize() const
    {
        return MaxStackSize;
    }

    /**
     * Checks if the specified item exists within the inventory container.
     *
     * This function determines whether the inventory container contains an item
     * with the provided identifier.
     *
     * @param ItemID The identifier of the item to check for in the inventory.
     * @return True if the item with the given identifier exists in the inventory, otherwise false.
     */
    UFUNCTION(BlueprintPure, Category = "Inventory")
    bool HasItem(FName ItemID) const;

    /**
     * Checks whether the inventory container is empty.
     *
     * This method verifies if the container has no items in its slots.
     *
     * @return True if the inventory container is empty, false otherwise.
     */
    UFUNCTION(BlueprintPure, Category = "Inventory")
    bool IsEmpty() const;

    /**
     * Retrieves the map of items contained in the inventory.
     *
     * The returned map represents item entries, where the key is the item's name
     * and the value is the corresponding quantity of that item.
     *
     * @return A read-only reference to a map containing item names as keys and their quantities as values.
     */
    const TMap<FName, int32> &GetItems() const
    {
        return ItemSlots;
    }

  protected:
    /**
     * Sets the maximum number of slots for the inventory container.
     *
     * This function updates the maximum allowable slots for the inventory container
     * based on the provided optional value.
     *
     * @param InMaxSlots The new maximum number of slots to be set. This is an optional value.
     */
    void SetMaxSlots(const TOptional<int32> &InMaxSlots)
    {
        MaxSlots = InMaxSlots;
    }

    /**
     * Sets the maximum stack size for the inventory.
     *
     * This method updates the maximum number of items allowed in a single stack
     * within the inventory.
     *
     * @param InMaxStackSize The optional value representing the maximum stack size.
     *                       If not set, the stack size may remain unset or defaulted.
     */
    void SetMaxStackSize(const TOptional<int32> &InMaxStackSize)
    {
        MaxStackSize = InMaxStackSize;
    }

    /**
     * Determines if an item can be added to the inventory container.
     *
     * This method checks whether the specified item can be added to the inventory
     * container based on its ID, the amount being added, and any applicable
     * constraints such as max slots or max stack size.
     *
     * @param ItemID The identifier of the item to be added.
     * @param Amount The quantity of the item to be added.
     * @return true if the item can be added, false otherwise.
     */
    virtual bool NativeCanAddItem(FName ItemID, int32 Amount) const;

    /**
     * Validates whether the specified item ID is valid for use in the inventory system.
     *
     * This method checks if the given item ID is not empty or unassigned (None).
     *
     * @param ItemID The unique identifier of the item to validate.
     * @return True if the item ID is valid; otherwise, false.
     */
    virtual bool NativeValidateItem(FName ItemID) const;

    /**
     * Determines whether an item can be added to the inventory.
     *
     * This function checks the specified item ID and quantity to evaluate if the
     * item can be added to the inventory.
     *
     * @param ItemID The unique identifier of the item to be added.
     * @param Amount The quantity of the item to be added.
     * @return The validation status indicating whether the item can be added successfully.
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "Inventory", meta = (ScriptName = "CanAddItem"))
    EInventoryValidation K2_CanAddItem(FName ItemID, int32 Amount) const;

    /**
     * Blueprint implementable event for validating an item within the inventory.
     *
     * This function is intended to be overridden in Blueprint to provide custom validation logic
     * for checking the status of an item specified by its ItemID.
     *
     * @param ItemID The unique identifier of the item to validate.
     * @return The validation status of the item, as an EInventoryValidation enum value.
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "Inventory", meta = (ScriptName = "ValidateItem"))
    EInventoryValidation K2_ValidateItem(FName ItemID) const;

  private:
    UPROPERTY(SaveGame)
    TMap<FName, int32> ItemSlots;

    /**
     * Optional integer specifying the maximum number of slots available.
     *
     * This property defines the upper limit of slots that can be configured
     * for use. If not set, no limit is enforced.
     */
    UPROPERTY(EditAnywhere, Category = "Settings")
    TOptional<int32> MaxSlots;

    /**
     * Represents the maximum stack size for items in the inventory container.
     *
     * This property determines the upper limit for the number of items that can be
     * stacked together. If not set, the stack size may default to other settings
     * or behaviors defined elsewhere.
     */
    UPROPERTY(EditAnywhere, Category = "Settings")
    TOptional<int32> MaxStackSize;
};
