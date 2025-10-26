using Pokemon.Data.Model.PBS;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.RPGCore;

namespace Pokemon.Core.Components.Bag;

/// <summary>
/// Provides functionality for managing the registration of items within a component.
/// </summary>
/// <remarks>
/// Designed to be used as part of an RPG system for managing item registrations. This component allows
/// items to be registered or unregistered and verifies if a specific item is currently registered.
/// </remarks>
[UClass]
public partial class UItemRegistrationComponent : URPGComponent
{
    /// <summary>
    /// Represents the collection of items that are currently registered within the component.
    /// </summary>
    /// <remarks>
    /// This property stores a set of items using their respective handles. It ensures that only unique items
    /// are registered and allows for efficient lookup operations.
    /// </remarks>
    [UProperty]
    public partial TSet<FItemHandle> RegisteredItems { get; }

    /// <summary>
    /// Determines whether a specific item is registered within the component.
    /// </summary>
    /// <param name="item">The item handle to check for registration.</param>
    /// <returns>
    /// True if the specified item is registered; otherwise, false.
    /// </returns>
    [UFunction(FunctionFlags.BlueprintPure, Category = "Inventory|Registration")]
    public bool IsRegistered(FItemHandle item) => RegisteredItems.Contains(item);

    /// <summary>
    /// Registers a specific item within the component.
    /// </summary>
    /// <param name="item">The item handle to be registered.</param>
    [UFunction(FunctionFlags.BlueprintCallable, Category = "Inventory|Registration")]
    public void RegisterItem(FItemHandle item) => RegisteredItems.Add(item);

    /// <summary>
    /// Removes the specified item from the list of registered items in the component.
    /// </summary>
    /// <param name="item">The item handle to unregister from the component.</param>
    [UFunction(FunctionFlags.BlueprintCallable, Category = "Inventory|Registration")]
    public void UnregisterItem(FItemHandle item) => RegisteredItems.Remove(item);
}
