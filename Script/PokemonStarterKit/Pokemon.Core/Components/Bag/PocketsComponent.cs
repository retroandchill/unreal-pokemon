using JetBrains.Annotations;
using Pokemon.Core.Bag;
using Pokemon.Core.Entities;
using Pokemon.Data.Model.PBS;
using RPG.SourceGenerator.Attributes;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.GameplayTags;
using UnrealSharp.RPGCore;

namespace Pokemon.Core.Components.Bag;

/// <summary>
/// Represents a component for managing item pockets in a game.
/// Extends the base functionality of URPGComponent to handle inventory systems with various pockets and items.
/// </summary>
[UClass]
public class UPocketsComponent : URPGComponent
{
    [UProperty]
    [UsedImplicitly]
    private TMap<FGameplayTag, UBagPocket> Pockets { get; }

    /// <summary>
    /// Gets an enumerable collection of all pockets within the bag. Each pocket is represented
    /// by an identifier that includes the pocket's unique gameplay tag and its display name.
    /// This property provides a way to iterate over all defined pockets in the bag, enabling
    /// functionality such as pocket-specific operations or filtering.
    ///</summary>
    public IEnumerable<FPocketIdentifier> AllPockets => Pockets.Select(x => new FPocketIdentifier(x.Key, x.Value.Name));

    /// <summary>
    /// Gets the total number of pockets currently defined within the component. Each pocket
    /// is managed internally with its own unique gameplay tag and associated metadata.
    /// This property provides a simple mechanism to retrieve the count of all pockets in the component.
    /// </summary>
    public int PocketCount => Pockets.Count;

    /// <summary>
    /// Gets a lookup collection of all items grouped by their corresponding gameplay tags.
    /// Each entry contains a tag representing the category of items and a collection of item slots,
    /// where each slot includes the item handle and its quantity.
    /// This property facilitates efficient retrieval and categorization of items within the bag.
    /// </summary>
    public ILookup<FGameplayTag, FItemSlot> AllItems
    {
        get
        {
            return Pockets
                .SelectMany(x =>
                    x.Value.Items.Select(y => new FItemSlot(y.Key, y.Value)).Select(z => (Tag: x.Key, Item: z))
                )
                .ToLookup(x => x.Tag, x => x.Item);
        }
    }

    /// <summary>
    /// Initializes the component by setting up pockets based on the provided initialization parameters.
    /// Each pocket is configured with a name, tag, maximum size, and maximum items per slot.
    /// </summary>
    /// <param name="initParams">The initialization parameters containing pocket settings and maximum item capacity per slot.</param>
    [UFunction]
    [ExcludeFromExtensions]
    public virtual void Initialize(FBagInitParams initParams)
    {
        foreach (var (tag, name, maxSize) in initParams.Pockets)
        {
            var pocket = NewObject<UBagPocket>(this);
            pocket.MaxSlots = maxSize;
            pocket.MaxStackSize = initParams.MaxItemsPerSlot;
            pocket.Name = name;
            pocket.Tag = tag;
            Pockets.Add(tag, pocket);
        }
    }

    /// <summary>
    /// Adds a specified quantity of an item to the appropriate pocket in the inventory.
    /// If the item already exists in the pocket, the quantity is incremented. If not, a new entry is created.
    /// </summary>
    /// <param name="item">The handle of the item to be added to the inventory.</param>
    /// <param name="quantity">The quantity of the item to be added. Defaults to 1 if not specified.</param>
    /// <returns>The total quantity of the item in the pocket after the addition.</returns>
    [UFunction(FunctionFlags.BlueprintCallable, Category = "Inventory")]
    public int AddItem(FItemHandle item, int quantity = 1)
    {
        return GetPocket(item).AddItem(item, quantity);
    }

    /// <summary>
    /// Attempts to add the exact specified quantity of an item to the appropriate pocket in the inventory.
    /// This method ensures that the total quantity of the item does not exceed the pocket's maximum stack size.
    /// If the addition would exceed the limit, no items are added.
    /// </summary>
    /// <param name="item">The handle representing the item to be added to the inventory.</param>
    /// <param name="quantity">The exact quantity of the item to be added. Defaults to 1 if not specified.</param>
    /// <returns>
    /// The quantity of the item successfully added to the inventory. Returns 0 if the addition would exceed the maximum stack size.
    /// </returns>
    [UFunction(FunctionFlags.BlueprintCallable, Category = "Inventory")]
    public int AddItemExact(FItemHandle item, int quantity = 1)
    {
        var pocket = GetPocket(item);
        var maxStackSize = pocket.MaxStackSize.Match(x => x, () => int.MaxValue);
        return maxStackSize - pocket.GetItemQuantity(item) >= quantity ? AddItem(item, quantity) : 0;
    }

    /// <summary>
    /// Removes the specified quantity of an item from the appropriate pocket in the inventory system.
    /// If the item does not exist in the inventory or the quantity to remove exceeds the available quantity,
    /// the method adjusts the removed quantity accordingly.
    /// </summary>
    /// <param name="item">The handle representing the item to be removed from the inventory.</param>
    /// <param name="quantity">The number of units of the item to remove. Defaults to 1 if not specified.</param>
    /// <returns>The actual quantity of the item removed from the inventory.</returns>
    [UFunction(FunctionFlags.BlueprintCallable, Category = "Inventory")]
    public int RemoveItem(FItemHandle item, int quantity = 1)
    {
        return GetPocket(item).RemoveItem(item, quantity);
    }

    /// <summary>
    /// Removes an exact quantity of a specific item from the appropriate pocket, if the required quantity is available.
    /// </summary>
    /// <param name="item">The handle of the item to be removed.</param>
    /// <param name="quantity">The exact quantity of the item to remove.</param>
    /// <returns>The number of items successfully removed. Returns 0 if the required quantity is not available.</returns>
    [UFunction(FunctionFlags.BlueprintCallable, Category = "Inventory")]
    public int RemoveItemExact(FItemHandle item, int quantity = 1)
    {
        var pocket = GetPocket(item);
        return pocket.GetItemQuantity(item) >= quantity ? RemoveItem(item, quantity) : 0;
    }

    /// <summary>
    /// Replaces an item in the corresponding pocket with another item, ensuring both items belong to the same pocket category.
    /// </summary>
    /// <param name="item1">The item to be replaced. Must belong to a valid pocket and match the pocket category of the replacement item.</param>
    /// <param name="item2">The item to replace with. Must belong to a valid pocket and match the pocket category of the original item.</param>
    /// <returns>Returns true if the replacement is successful; otherwise, returns false.</returns>
    [UFunction(FunctionFlags.BlueprintCallable, Category = "Inventory")]
    public bool ReplaceItem(FItemHandle item1, FItemHandle item2)
    {
        if (!item1.IsValid || !item2.IsValid || item1.Entry.Pocket != item2.Entry.Pocket)
            return false;

        return GetPocket(item1).ReplaceItem(item1, item2);
    }

    /// <summary>
    /// Retrieves the quantity of a specified item from the relevant pocket.
    /// </summary>
    /// <param name="item">The handle of the item whose quantity is to be retrieved.</param>
    /// <returns>The quantity of the specified item currently stored in the appropriate pocket.</returns>
    [UFunction(FunctionFlags.BlueprintPure, Category = "Inventory")]
    public int GetItemQuantity(FItemHandle item)
    {
        return GetPocket(item).GetItemQuantity(item);
    }

    /// <summary>
    /// Determines whether the specified item exists in the pockets managed by this component.
    /// </summary>
    /// <param name="item">The handle representing the item to check for existence in the inventory.</param>
    /// <returns>True if the item exists in the inventory, otherwise false.</returns>
    [UFunction(FunctionFlags.BlueprintPure, Category = "Inventory")]
    public bool HasItem(FItemHandle item)
    {
        return GetPocket(item).HasItem(item);
    }

    /// <summary>
    /// Retrieves the pocket associated with the specified item.
    /// Ensures the item is valid and maps to an existing pocket; otherwise, throws an exception.
    /// </summary>
    /// <param name="item">The item handle used to identify the associated pocket.</param>
    /// <returns>The pocket where the specified item belongs.</returns>
    /// <exception cref="ArgumentException">Thrown when the item is invalid or does not map to a valid pocket tag.</exception>
    [ExcludeFromExtensions]
    public UBagPocket GetPocket(FItemHandle item)
    {
        if (!item.IsValid)
        {
            throw new ArgumentException("Item is not valid");
        }

        return Pockets.TryGetValue(item.Entry.Pocket, out var pocket)
            ? pocket
            : throw new ArgumentException($"Invalid item pocket tag: {item.Entry.Pocket}");
    }
}
