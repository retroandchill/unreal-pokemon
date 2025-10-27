using InteractiveUI.Core.Selection;
using Pokemon.Core.Entities;
using Pokemon.Data.Model.PBS;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.GameplayTags;
using UnrealSharp.InteractiveUI;

namespace Pokemon.UI.Components.Bag;

/// <summary>
/// Represents a window component for item selection within the context of
/// a Pokémon bag. This class manages item interactions and displays
/// filtered sets of items, allowing for selection and hover operations.
/// </summary>
[UClass(ClassFlags.Abstract)]
public partial class UItemSelectionWindow : UOwningSelectionWidget
{
    /// <summary>
    /// Represents the class type to be used for constructing individual item entries
    /// within the item selection window. Each item entry represents a selectable
    /// item option in the Bag UI.
    /// </summary>
    /// <remarks>
    /// This property is leveraged during dynamic widget creation to instantiate
    /// objects of the specified item entry class. The class must inherit from
    /// <c>UItemOption</c> and define item-related behaviors and display logic.
    /// </remarks>
    [UProperty(PropertyFlags.EditAnywhere, Category = "Display")]
    private partial TSubclassOf<UItemOption> ItemEntryClass { get; }

    /// <summary>
    /// Event triggered when an item is hovered over in the item selection window.
    /// This event provides details about the currently hovered item and its quantity.
    /// </summary>
    /// <remarks>
    /// Invoked whenever the hover operation occurs within the item selection widget,
    /// such as when navigating through items. The <c>FItemHandle</c> parameter refers
    /// to the specific item being hovered, and the <c>int</c> parameter represents the
    /// quantity of the item.
    /// </remarks>
    public event Action<FItemHandle, int>? OnItemHovered;

    /// <summary>
    /// Event triggered when an item is selected within the item selection window.
    /// </summary>
    /// <remarks>
    /// This event provides the selected item's handle and its quantity as parameters.
    /// It is utilized to notify external systems when a user finalizes an item selection.
    /// </remarks>
    public event Action<FItemHandle, int>? OnItemSelected;

    private Func<FItemHandle, bool>? _itemFilter;

    /// <summary>
    /// Represents the Pokémon Bag used as a data source for item management within
    /// the item selection window. This property provides access to the collection
    /// of items, enabling filtering, interaction, and display logic for the UI.
    /// </summary>
    /// <remarks>
    /// This property is utilized to fetch relevant items grouped by pockets or categories
    /// within the Bag UI. It serves as a bridge between the underlying item data and
    /// the dynamic UI component that displays and processes the selected items.
    /// </remarks>
    [UProperty]
    public partial UPokemonBag Bag { get; set; }

    /// <summary>
    /// Represents the currently selected pocket within the item selection window.
    /// Used to determine which category of items, such as healing items or Poké Balls,
    /// is displayed in the bag UI.
    /// </summary>
    /// <remarks>
    /// Assigning a value to this property triggers the bag UI to update and display
    /// the corresponding filtered set of items related to the selected pocket.
    /// This property integrates with the item filtering and display logic to
    /// ensure the interface reflects the current selection.
    /// </remarks>
    public FGameplayTag CurrentPocket
    {
        get;
        set
        {
            field = value;
            UpdatePocket();
        }
    }

    /// <inheritdoc />
    protected override void Construct_Implementation()
    {
        base.Construct_Implementation();
        Buttons.OnHoveredButtonBaseChanged += [UFunction] (_, i) => OnSelectionChange(i);
        Buttons.OnButtonBaseClicked += [UFunction] (_, i) => ProcessConfirm(i);
    }

    /// <summary>
    /// Updates the item selection window with the current pocket's item data.
    /// Ensures the filtering logic and UI display are refreshed to reflect the
    /// most recent state of the bag's content within the defined pocket.
    /// </summary>
    public void Refresh()
    {
        UpdatePocket();
    }

    /// <summary>
    /// Applies a filter function to determine which items are displayed in the selection window.
    /// This filter is used to show a subset of items based on specific criteria.
    /// </summary>
    /// <param name="filter">A function that takes an <see cref="FItemHandle"/> as input and returns
    /// a boolean indicating whether the item meets the filter criteria.</param>
    public void ApplyItemFilter(Func<FItemHandle, bool> filter)
    {
        _itemFilter = filter;
        UpdatePocket();
    }

    /// <summary>
    /// Resets the item filtering behavior by clearing the current item filter
    /// and updating the pocket to reflect unfiltered items.
    /// </summary>
    public void ResetItemFilter()
    {
        _itemFilter = null;
        UpdatePocket();
    }

    private void OnSelectionChange(int index)
    {
        if (Buttons.GetButtonBaseAtIndex(index) is UItemOption option)
        {
            OnItemHovered?.Invoke(option.Item, option.Quantity);
        }
        else if (Buttons.ButtonCount == 0)
        {
            OnItemHovered?.Invoke(default, 0);
        }
    }

    /// <summary>
    /// Processes a confirmation action by retrieving the item option at the specified index,
    /// triggering hover and selection events for the associated item.
    /// </summary>
    /// <param name="currentIndex">The zero-based index corresponding to the item option to be confirmed.</param>
    /// <exception cref="InvalidOperationException">Thrown if there is no item option at the specified index.</exception>
    protected void ProcessConfirm(int currentIndex)
    {
        if (Buttons.GetButtonBaseAtIndex(currentIndex) is not UItemOption option)
            throw new InvalidOperationException("No option at index");
        OnItemHovered?.Invoke(option.Item, option.Quantity);
        OnItemSelected?.Invoke(option.Item, option.Quantity);
    }

    private void UpdatePocket()
    {
        Buttons.RemoveAll();
        foreach (var (item, quantity) in Bag.AllItems[CurrentPocket])
        {
            if (_itemFilter is not null && !_itemFilter(item))
                return;

            var option = WidgetTree.ConstructWidget(ItemEntryClass);
            option.SetItem(item, quantity);
            Buttons.AddWidget(option);
        }

        DesiredFocusIndex = Buttons.ButtonCount > 0 ? Math.Clamp(DesiredFocusIndex, 0, Buttons.ButtonCount - 1) : 0;

        var desiredFocusTarget = DesiredFocusTarget;
        if (IsActive)
        {
            desiredFocusTarget?.SetFocus();
        }

        if (desiredFocusTarget is UItemOption itemOption)
        {
            OnItemHovered?.Invoke(itemOption.Item, itemOption.Quantity);
        }
    }
}
