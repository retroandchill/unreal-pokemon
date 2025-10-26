using Pokemon.Data.Model.PBS;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.RPGCore;

namespace Pokemon.Core.Components.Pokemon;

/// <summary>
/// Represents a collection of items that a wild Pokémon can hold categorized by their rarity.
/// </summary>
/// <remarks>
/// This struct stores three separate lists of items that are classified based on their rarity:
/// common, uncommon, and rare. Each list contains item handles referencing the respective items.
/// </remarks>
[UStruct]
public readonly partial record struct FWildHoldItems(
    [property: UProperty(PropertyFlags.BlueprintReadOnly)] IReadOnlyList<FItemHandle> Common,
    [property: UProperty(PropertyFlags.BlueprintReadOnly)] IReadOnlyList<FItemHandle> Uncommon,
    [property: UProperty(PropertyFlags.BlueprintReadOnly)] IReadOnlyList<FItemHandle> Rare
);

/// <summary>
/// Represents a component for managing and interacting with the item held by a Pokémon.
/// </summary>
/// <remarks>
/// This component handles the assignment, verification, and retrieval of held items for a Pokémon.
/// It provides functionality to check if a Pokémon holds an item, determine specific held items,
/// and retrieve data about items available in the wild categorized by rarity.
/// </remarks>
[UClass]
[UMetaData("HideCategories", "HoldItem")]
public partial class UHoldItemComponent : URPGComponent
{
    [UProperty(PropertyFlags.Transient)]
    private partial UIdentityComponent IdentityComponent { get; set; }

    /// <summary>
    /// Gets or sets the item held by the Pokémon.
    /// </summary>
    /// <remarks>
    /// The property encapsulates the held item's handle, enabling assignment and retrieval.
    /// If an invalid handle is set, the property silently rejects it. This ensures the validity
    /// of the held item at all times.
    /// </remarks>
    [UProperty(PropertyFlags.BlueprintReadWrite, Category = "HoldItem", BlueprintAccessors = true)]
    public TOptional<FItemHandle> Item
    {
        get => Item_BackingField;
        set
        {
            if (value.HasValue && !value.Value.IsValid)
                return;

            Item_BackingField = value;
        }
    }

    /// <summary>
    /// Indicates whether the Pokémon currently holds an item.
    /// </summary>
    /// <remarks>
    /// This property evaluates to true if the Pokémon has a valid item assigned through its held item component.
    /// Otherwise, it evaluates to false. This provides a quick way to check the existence of a held item
    /// without requiring direct access to the underlying item handle or its state.
    /// </remarks>
    public bool HasItem
    {
        [UFunction(FunctionFlags.BlueprintPure, DisplayName = "Has Item", Category = "HoldItem")]
        get => Item.HasValue;
    }

    /// <summary>
    /// Gets a collection of items potentially held by Pokémon in the wild, categorized by rarity.
    /// </summary>
    /// <remarks>
    /// The property returns an immutable structure encapsulating lists of item handles grouped into
    /// common, uncommon, and rare categories. These lists define the possible items that Pokémon in
    /// the wild may hold during encounters, providing a breakdown of their rarity distribution.
    /// </remarks>
    public FWildHoldItems WildHoldItems
    {
        [UFunction(FunctionFlags.BlueprintPure, DisplayName = "Wild Hold Items", Category = "HoldItem")]
        get
        {
            var species = IdentityComponent.Species.Entry;
            return new FWildHoldItems(
                species.WildHoldItemCommon.ToArray(),
                species.WildHoldItemUncommon.ToArray(),
                species.WildHoldItemRare.ToArray()
            );
        }
    }

    /// <inheritdoc />
    protected override void PreInitialize_Implementation()
    {
        IdentityComponent = GetRequiredSiblingComponent<UIdentityComponent>();
    }

    /// <summary>
    /// Determines whether the Pokémon holds a specific item.
    /// </summary>
    /// <param name="item">The item to check for comparison with the held item.</param>
    /// <returns>True if the Pokémon holds the specified item; otherwise, false.</returns>
    [UFunction(FunctionFlags.BlueprintPure, DisplayName = "Has Specific Item", Category = "HoldItem")]
    public bool HasSpecificItem(FItemHandle item)
    {
        return Item.Match(x => x == item, () => false);
    }
}
