using Pokemon.Core.Entities;
using Pokemon.Data;
using RPG.SourceGenerator.Attributes;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.Attributes.MetaTags;
using UnrealSharp.GameplayTags;
using UnrealSharp.RPGCore;

namespace Pokemon.Core.Components.Bag;

/// <summary>
/// Represents the memory component responsible for tracking the last viewed pockets and items
/// in the inventory system of a player's bag.
/// </summary>
/// <remarks>
/// This component maintains the state of the pocket last viewed by the user
/// and records the index of the last viewed items within each pocket. It is initialized
/// during the setup of the associated bag entity and interacts with related inventory
/// systems to provide a seamless user experience.
/// </remarks>
[UClass]
[UMetaData("HideCategories", "Inventory")]
public partial class UPocketMemoryComponent : URPGComponent
{
    [UProperty(PropertyFlags.Transient)]
    private partial UPocketsComponent PocketsComponent { get; set; }

    /// <summary>
    /// Gets or sets the <see cref="FGameplayTag"/> representing the last pocket viewed by the user.
    /// </summary>
    /// <remarks>
    /// This property provides the mechanism to track the most recently accessed pocket in the inventory system.
    /// It is updated whenever the user navigates to or interacts with a pocket. The value is used
    /// internally to maintain a seamless navigation flow within the inventory.
    /// </remarks>
    [UProperty(PropertyFlags.BlueprintReadWrite, Category = "Inventory")]
    public partial FGameplayTag LastViewedPocket { get; set; }

    /// <summary>
    /// Gets the mapping of <see cref="FGameplayTag"/> representing pockets to their respective indices
    /// of the last viewed items within those pockets.
    /// </summary>
    /// <remarks>
    /// This property is used to track the index of the last accessed item for each pocket in the
    /// inventory system. It allows the system to restore the user's navigation context within each pocket
    /// when revisiting or interacting with items. Updates to this property occur as the user navigates or
    /// selects items in different pockets.
    /// </remarks>
    [UProperty(PropertyFlags.BlueprintReadOnly, Category = "Inventory")]
    public partial TMap<FGameplayTag, int> LastViewedItems { get; }

    /// <inheritdoc />
    protected override void PreInitialize_Implementation()
    {
        PocketsComponent = GetRequiredSiblingComponent<UPocketsComponent>();
    }

    /// <summary>
    /// Initializes the component using the provided parameters. This method should be
    /// called to set up any required initial state for the component.
    /// </summary>
    /// <param name="initParams">
    /// The parameters used to initialize the component, including settings and limits
    /// related to the bag functionality.
    /// </param>
    [UFunction]
    [ExcludeFromExtensions]
    public virtual void Initialize(FBagInitParams initParams)
    {
        ResetLastSelections();
    }

    /// <summary>
    /// Resets the last viewed selections for all pockets in the inventory.
    /// </summary>
    /// <remarks>
    /// This method sets the first pocket in the inventory as the default "last viewed pocket"
    /// and initializes the viewed index of all pockets to zero. It iterates through all the pockets
    /// managed by the connected <see cref="UPocketsComponent"/> and updates the state managed
    /// by <see cref="LastViewedPocket"/> and <see cref="LastViewedItems"/>.
    /// </remarks>
    [UFunction(FunctionFlags.BlueprintCallable, Category = "Inventory")]
    public void ResetLastSelections()
    {
        foreach (var (tag, i) in PocketsComponent.AllPockets.Select((x, i) => (x.Tag, i)))
        {
            if (i == 0)
            {
                LastViewedPocket = tag;
            }

            LastViewedItems.Add(tag, 0);
        }
    }

    /// <summary>
    /// Retrieves the last viewed index for the specified pocket tag.
    /// </summary>
    /// <remarks>
    /// Throws an <see cref="ArgumentException"/> if the provided pocket tag is not valid.
    /// </remarks>
    /// <param name="pocketTag">
    /// The gameplay tag identifying the pocket for which the last viewed index is to be retrieved.
    /// </param>
    /// <returns>
    /// The last viewed index associated with the specified pocket tag.
    /// </returns>
    [UFunction(FunctionFlags.BlueprintPure, Category = "Inventory")]
    public int GetLastViewedIndex([Categories(IdentifierConstants.PocketTag)] FGameplayTag pocketTag)
    {
        return LastViewedItems.TryGetValue(pocketTag, out var index)
            ? index
            : throw new ArgumentException($"Invalid pocket tag: {pocketTag}");
    }

    /// <summary>
    /// Sets the last viewed item index for the specified pocket tag.
    /// </summary>
    /// <param name="pocketTag">The tag identifying the pocket to update.</param>
    /// <param name="index">The index of the item to set as last viewed within the specified pocket.</param>
    /// <exception cref="ArgumentException">Thrown if the provided pocket tag is not valid.</exception>
    [UFunction(FunctionFlags.BlueprintCallable, Category = "Inventory")]
    public void SetLastViewedIndex([Categories(IdentifierConstants.PocketTag)] FGameplayTag pocketTag, int index)
    {
        if (!LastViewedItems.ContainsKey(pocketTag))
        {
            throw new ArgumentException($"Invalid pocket tag: {pocketTag}");
        }

        LastViewedItems[pocketTag] = index;
    }
}
