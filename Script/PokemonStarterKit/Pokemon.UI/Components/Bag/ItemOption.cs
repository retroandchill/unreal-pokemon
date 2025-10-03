using Pokemon.Data.Model.PBS;
using Pokemon.UI.Components.Common;
using UnrealSharp.Attributes;
using UnrealSharp.Attributes.MetaTags;
using UnrealSharp.CommonUI;
using UnrealSharp.UMG;

namespace Pokemon.UI.Components.Bag;

/// <summary>
/// Represents an item option button in the inventory system of the Bag UI.
/// Extends the functionality of a common button base and provides features for handling and displaying
/// inventory items, including their associated details such as quantity and properties.
/// </summary>
[UClass(ClassFlags.Abstract)]
public class UItemOption : UCommonButtonBase
{
    /// <summary>
    /// Gets the unique item handle associated with a specific inventory item.
    /// The item represents a reference to an in-game object defined in the game's item data repository.
    /// This property is read-only and is primarily used to identify items and manage their behavior or properties
    /// within the context of the inventory and UI system.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly, Category = "Inventory")]
    public FItemHandle Item { get; private set; }

    /// <summary>
    /// Gets the quantity associated with the current inventory item.
    /// This property indicates the number of units of a specific item in the player's inventory.
    /// It is used within the item management and UI systems for display and tracking purposes.
    /// This property is read-only.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly, Category = "Inventory")]
    public int Quantity { get; private set; }

    [UProperty]
    [BindWidgetOptional]
    private UItemDisplayBase? ItemIcon { get; }

    [UProperty]
    [BindWidget]
    private UCommonTextBlock ItemNameText { get; }

    [UProperty]
    [BindWidget]
    private UItemQuantityDisplayBase ItemQuantityText { get; }

    /// <summary>
    /// Sets the item and its quantity for this option.
    /// Updates the internal item reference and triggers the necessary handling when
    /// an item and its quantity are set.
    /// </summary>
    /// <param name="item">The handle to the item being set. This handle references the specific item data.</param>
    /// <param name="quantity">The quantity of the item being set.</param>
    [UFunction(FunctionFlags.BlueprintCallable, Category = "Inventory")]
    public void SetItem(FItemHandle item, int quantity)
    {
        Item = item;
        Quantity = quantity;
        OnItemSet(item, quantity);
    }

    /// <summary>
    /// Triggered whenever an item and its quantity are set or updated within the inventory UI.
    /// This method ensures that the item details are properly updated and displayed.
    /// </summary>
    /// <param name="item">The handle to the item that is being set or updated, representing specific item data.</param>
    /// <param name="quantity">The quantity of the specified item being set for display and usage purposes.</param>
    [UFunction(FunctionFlags.BlueprintEvent, Category = "Display")]
    protected virtual void OnItemSet(FItemHandle item, int quantity)
    {
        ItemIcon?.Item = item;
        ItemNameText.Text = item.Entry.Name;

        if (item.Entry.ShowQuantity)
        {
            ItemQuantityText.Visibility = ESlateVisibility.SelfHitTestInvisible;
            ItemQuantityText.Quantity = quantity;
        }
        else
        {
            ItemQuantityText.Visibility = ESlateVisibility.Collapsed;
        }
    }

    /// <inheritdoc />
    protected override void OnCurrentTextStyleChanged()
    {
        var currentStyle = CurrentTextStyleClass;
        ItemNameText.Style = currentStyle;
        ItemQuantityText.TextStyle = currentStyle;
    }
}
