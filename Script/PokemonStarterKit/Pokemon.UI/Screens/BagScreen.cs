using InteractiveUI.Core;
using InteractiveUI.Core.Selection;
using InteractiveUI.Core.Utilities;
using Pokemon.Core;
using Pokemon.Core.Entities;
using Pokemon.Core.Services.Async;
using Pokemon.Data.Model.PBS;
using Pokemon.UI.Components.Bag;
using Pokemon.UI.Components.Common;
using UnrealSharp.Attributes;
using UnrealSharp.Attributes.MetaTags;
using UnrealSharp.CommonUI;
using UnrealSharp.Engine;
using UnrealSharp.GameplayTags;
using UnrealSharp.InteractiveUI;
using UnrealSharp.UMG;

namespace Pokemon.UI.Screens;

/// <summary>
/// Represents the UI screen used for managing a bag in the game. This screen
/// allows players to view and interact with items in their bag, apply filters,
/// and perform item-related actions such as selection or usage.
/// Inherits from UCommonActivatableWidget to benefit from common functionality
/// for activatable widgets within the UI system.
/// </summary>
[UClass(ClassFlags.Abstract)]
public class UBagScreen : UCommonActivatableWidget
{
    /// <summary>
    /// Gets the currently selected or hovered item in the BagScreen UI.
    /// This property represents an instance of an item (FItemHandle) that is
    /// either interacted with, focused on, or highlighted in the current bag context.
    /// </summary>
    /// <remarks>
    /// The property is marked as read-only and transient, meaning it holds state information
    /// about the current session but does not persist across sessions. It allows the UI screen
    /// to relay contextual data about the item currently in focus for use in item-related
    /// animations, tooltips, or functionality.
    /// </remarks>
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.Transient, Category = "State")]
    public FItemHandle CurrentItem { get; private set; }

    [UProperty(PropertyFlags.Transient)]
    private UPokemonBag Bag { get; set; }

    /// <summary>
    /// Provides the UI component responsible for displaying and interacting with selectable items within the BagScreen.
    /// This property represents an instance of the UItemSelectionWindow widget, which manages user interactions,
    /// including navigation, selection, and hover states for items in the bag.
    /// </summary>
    /// <remarks>
    /// The ItemSelectionWindow is bound to the BagScreen and serves as a central interface for item-related operations.
    /// It enables functionalities such as item filtering, activation of contextual commands, and triggering relevant events
    /// (e.g., OnItemHovered, OnItemSelected, OnBackAction).
    /// This property is critical for maintaining the interactive and navigational flow of the BagScreen.
    /// </remarks>
    [UProperty(PropertyFlags.BlueprintReadOnly, Category = "Widgets")]
    [BindWidget]
    protected UItemSelectionWindow ItemSelectionWindow { get; }

    /// <summary>
    /// Represents the UI widget responsible for displaying detailed information
    /// about a specific item in the BagScreen UI. This widget provides contextual
    /// details such as an item's description, effects, or other attributes.
    /// </summary>
    /// <remarks>
    /// Used primarily to render additional visual and textual information related
    /// to the currently hovered or selected item. The visibility of this widget
    /// can be toggled based on user interactions with items in the bag.
    /// </remarks>
    [UProperty(PropertyFlags.BlueprintReadOnly, Category = "Widgets")]
    [BindWidget]
    protected UItemInfoWindow ItemInfoWindow { get; }

    /// <summary>
    /// Represents the widget responsible for managing and displaying the tab interface
    /// associated with item pockets in the BagScreen UI.
    /// </summary>
    /// <remarks>
    /// The PocketTabWidget enables navigation between different item pockets by allowing
    /// users to view and interact with categorized tabs. It plays a role in managing state
    /// changes when switching between pockets and integrates with other widgets such as the
    /// ItemSelectionWindow for seamless item selection and interaction within the Bag context.
    /// </remarks>
    [UProperty(PropertyFlags.BlueprintReadOnly, Category = "Widgets")]
    [BindWidget]
    protected UPocketTabWidget PocketTabWidget { get; }

    /// <summary>
    /// Represents the widget dedicated to displaying the contents of a specific pocket within the BagScreen UI.
    /// This property provides access to an instance of the `UPocketWindow` class, which is responsible for
    /// managing and rendering pocket-specific items and information.
    /// </summary>
    /// <remarks>
    /// The widget bound to this property plays a central role in displaying the inventory contents
    /// filtered by the currently active pocket, as determined by user input or application logic. It is used
    /// in combination with other components like `ItemSelectionWindow` to allow for an interactive and
    /// organized inventory navigation experience.
    /// </remarks>
    [UProperty(PropertyFlags.BlueprintReadOnly, Category = "Widgets")]
    [BindWidget]
    protected UPocketWindow PocketWindow { get; }

    /// <summary>
    /// Represents the widget responsible for displaying and managing commands or actions
    /// available to the player in the context of the current bag screen. This widget is
    /// activated when specific item-related commands or options need to be presented to
    /// the user.
    /// </summary>
    /// <remarks>
    /// This property facilitates the option selection or action confirmation workflows
    /// within the bag screen, using a focusable and activatable UI element. It is marked
    /// as readonly and supports binding to Blueprint widgets for integration into the
    /// game's UI design. The widget also integrates with the system to ensure an intuitive
    /// focus flow when transitioning between UI components.
    /// </remarks>
    [UProperty(PropertyFlags.BlueprintReadOnly, Category = "Widgets")]
    [BindWidget]
    protected USelectionWidget CommandWidget { get; }

    /// <summary>
    /// Represents the Pokemon selection UI pane within the BagScreen interface.
    /// This widget allows players to choose a specific Pokemon as part of item-related
    /// interactions or gameplay scenarios, such as using items on a Pokemon or organizing
    /// their Pokemon in certain contexts.
    /// </summary>
    /// <remarks>
    /// This property is marked as read-only and may optionally be bound, providing
    /// flexibility for its presence in different implementations of the BagScreen UI.
    /// The PokemonSelectionPane integrates seamlessly with the BagScreen's other widgets
    /// and expands its functionality to support Pokemon-specific operations. Its type,
    /// UPokemonSelectionWidgetBase, acts as a base for the selection interface, leveraging
    /// shared functionality for ownership and selection within the game UI system.
    /// </remarks>
    [UProperty(PropertyFlags.BlueprintReadOnly, Category = "Widgets")]
    [BindWidgetOptional]
    protected UPokemonSelectionWidgetBase? PokemonSelectionPane { get; }

    private Action<FItemSlotInfo?>? _onItemSelected;

    /// Pushes the BagScreen widget to the game menu layer stack asynchronously.
    /// <param name="playerController">
    /// The player controller responsible for managing the game interface.
    /// </param>
    /// <param name="cancellationToken">
    /// A token to monitor for cancellation requests during the asynchronous operation. Optional.
    /// </param>
    /// <returns>
    /// A task representing the asynchronous operation that, when completed, returns the instance of the BagScreen widget pushed to the stack.
    /// </returns>
    [UFunction(FunctionFlags.BlueprintCallable, Category = "Screens")]
    public static async Task<UBagScreen> PushBagScreenToStackAsync(
        APlayerController playerController,
        CancellationToken cancellationToken = default
    )
    {
        var screenClass = GetDefault<UPokemonUISettings>().BagScreenClass;
        return await playerController.PushContentToLayerAsync(
            GameplayTags.UI_Layer_GameMenu,
            screenClass,
            cancellationToken
        );
    }

    /// <inheritdoc />
    public override void Construct()
    {
        Bag = GetGameInstanceSubsystem<UPokemonSubsystem>().Bag;
        ItemSelectionWindow.Bag = Bag;
        PocketTabWidget.OnPocketChanged += OnPocketTabChanged;
        ItemSelectionWindow.OnItemHovered += SetHoveredItem;
        ItemSelectionWindow.OnItemSelected += SelectItem;
        ItemSelectionWindow.OnBackAction += OnItemSelectionCanceled;
        ItemSelectionWindow.OnWidgetActivated += [UFunction] () => PocketTabWidget.ActivateWidget();
        ItemSelectionWindow.OnWidgetDeactivated += [UFunction] () => PocketTabWidget.DeactivateWidget();

        OnPocketTabChanged(PocketTabWidget.CurrentPocket);
    }

    private void OnPocketTabChanged(FGameplayTag tag)
    {
        PocketWindow.CurrentPocket = tag;
        ItemSelectionWindow.CurrentPocket = tag;

        ItemSelectionWindow.DesiredFocusIndex = Bag.LastViewedItems[tag];
        var currentButton = ItemSelectionWindow.GetButton<UItemOption>(ItemSelectionWindow.DesiredFocusIndex);
        if (currentButton is not null)
        {
            SetHoveredItem(currentButton.Item, currentButton.Quantity);
        }
        else
        {
            SetHoveredItem(default, 0);
        }

        // Quickly deactivate and reactivate the widget to force the desired focus index to be updated
        if (ItemSelectionWindow.IsActive)
        {
            ItemSelectionWindow.DeactivateWidget();
        }

        ItemSelectionWindow.ActivateWidget();
    }

    /// <summary>
    /// Applies a filter to the list of items displayed in the item selection window.
    /// </summary>
    /// <param name="filter">
    /// A function that takes an FItemHandle as input and returns a boolean indicating whether the item
    /// should be included in the filtered list.
    /// </param>
    public void ApplyItemFilter(Func<FItemHandle, bool> filter)
    {
        ItemSelectionWindow.ApplyItemFilter(filter);
    }

    /// <summary>
    /// Resets the applied item filter within the BagScreen, restoring the default filtering state.
    /// This operation clears any custom conditions applied by the user or programmatically
    /// without directly affecting the underlying items.
    /// Internally, it delegates the reset operation to the associated ItemSelectionWindow
    /// to ensure consistencies across item displays.
    /// </summary>
    public void ResetItemFilter()
    {
        ItemSelectionWindow.ResetItemFilter();
    }

    /// <summary>
    /// Prompts the user to select an item asynchronously from the BagScreen widget.
    /// </summary>
    /// <returns>
    /// A task representing the asynchronous operation that, when completed, returns the information
    /// about the item slot selected by the user, or null if no item was selected.
    /// </returns>
    public Task<FItemSlotInfo?> PromptItemSelection()
    {
        if (_onItemSelected is not null)
        {
            throw new InvalidOperationException(
                "Cannot call PromptItemSelection when a callback is already registered"
            );
        }

        var tcs = new TaskCompletionSource<FItemSlotInfo?>();

        _onItemSelected = s =>
        {
            tcs.SetResult(s);
            _onItemSelected = null;
        };

        return tcs.Task;
    }

    /// <summary>
    /// Toggles the ability to select an item by activating or deactivating the item selection widget.
    /// </summary>
    /// <param name="canSelect">
    /// A boolean value indicating whether the item selection widget should be activated (true) or deactivated (false).
    /// </param>
    [UFunction(FunctionFlags.BlueprintCallable, Category = "Items|Selection")]
    public void ToggleItemSelection(bool canSelect)
    {
        if (canSelect)
        {
            ItemSelectionWindow.ActivateWidget();
        }
        else
        {
            ItemSelectionWindow.DeactivateWidget();
        }
    }

    /// <summary>
    /// Refreshes the BagScreen's item selection window by invoking the associated
    /// refresh method on the ItemSelectionWindow instance. This ensures the user
    /// interface is updated to reflect the current state of items and any changes
    /// that may have occurred.
    /// </summary>
    [UFunction(FunctionFlags.BlueprintCallable, Category = "Items|Selection")]
    public void Refresh()
    {
        ItemSelectionWindow.Refresh();
    }

    private void SetHoveredItem(FItemHandle item, int quantity)
    {
        CurrentItem = item;
        Bag.SetLastViewedIndex(ItemSelectionWindow.CurrentPocket, ItemSelectionWindow.DesiredFocusIndex);

        if (CurrentItem.IsValid)
        {
            ItemInfoWindow.Visibility = ESlateVisibility.SelfHitTestInvisible;
            ItemInfoWindow.Item = item;
        }
        else
        {
            ItemInfoWindow.Visibility = ESlateVisibility.Hidden;
        }
    }

    private void SelectItem(FItemHandle itemHandle, int quantity)
    {
        if (_onItemSelected is not null)
        {
            _onItemSelected(new FItemSlotInfo(itemHandle, quantity));
            return;
        }

        SetHoveredItem(itemHandle, quantity);
        ToggleItemSelection(false);
        ShowItemCommands();
    }

    [UFunction(FunctionFlags.BlueprintCallable, Category = "Items|Selection")]
    private void OnItemSelectionCanceled()
    {
        if (_onItemSelected is not null)
        {
            _onItemSelected(null);
            return;
        }

        this.PopContentFromLayer();
    }

    private void ShowItemCommands()
    {
        ItemSelectionWindow.DeactivateWidget();

        if (CommandWidget is IRefreshable refreshable)
        {
            refreshable.Refresh();
        }

        CommandWidget.DesiredFocusIndex = 0;
        CommandWidget.ActivateWidget();
    }

    /// <inheritdoc />
    protected override UWidget? BP_GetDesiredFocusTarget()
    {
        return FocusUtilities.GetFirstActivatedWidget(CommandWidget, ItemSelectionWindow);
    }

    // TODO: Using item events
}
