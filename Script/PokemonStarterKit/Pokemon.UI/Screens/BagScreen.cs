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

[UClass(ClassFlags.Abstract)]
public class UBagScreen : UCommonActivatableWidget
{
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.Transient, Category = "State")]
    public FItemHandle CurrentItem { get; private set; }

    [UProperty(PropertyFlags.Transient)]
    private UPokemonBag Bag { get; set; }

    [UProperty(PropertyFlags.BlueprintReadOnly, Category = "Widgets")]
    [BindWidget]
    protected UItemSelectionWindow ItemSelectionWindow { get; }

    [UProperty(PropertyFlags.BlueprintReadOnly, Category = "Widgets")]
    [BindWidget]
    protected UItemInfoWindow ItemInfoWindow { get; }

    [UProperty(PropertyFlags.BlueprintReadOnly, Category = "Widgets")]
    [BindWidget]
    protected UPocketTabWidget PocketTabWidget { get; }

    [UProperty(PropertyFlags.BlueprintReadOnly, Category = "Widgets")]
    [BindWidget]
    protected UPocketWindow PocketWindow { get; }

    [UProperty(PropertyFlags.BlueprintReadOnly, Category = "Widgets")]
    [BindWidget]
    protected USelectionWidget CommandWidget { get; }

    [UProperty(PropertyFlags.BlueprintReadOnly, Category = "Widgets")]
    [BindWidgetOptional]
    protected UPokemonSelectionWidgetBase? PokemonSelectionPane { get; }

    private Action<FItemSlotInfo?>? _onItemSelected;

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

    public override void Construct()
    {
        Bag = GetGameInstanceSubsystem<UPokemonSubsystem>().Bag;
        ItemSelectionWindow.Bag = Bag;
        PocketTabWidget.OnPocketChanged += OnPocketTabChanged;
        ItemSelectionWindow.OnItemHovered += SetHoveredItem;
        ItemSelectionWindow.OnItemSelected += SelectItem;
        ItemSelectionWindow.OnBackAction += OnItemSelectionCanceled;
        ItemSelectionWindow.OnWidgetActivated += [UFunction] () => PocketTabWidget.ActivateWidget();
        ItemSelectionWindow.OnWidgetDeactivated += [UFunction]
        () => PocketTabWidget.DeactivateWidget();

        OnPocketTabChanged(PocketTabWidget.CurrentPocket);
    }

    private void OnPocketTabChanged(FGameplayTag tag)
    {
        PocketWindow.CurrentPocket = tag;
        ItemSelectionWindow.CurrentPocket = tag;

        ItemSelectionWindow.DesiredFocusIndex = Bag.LastViewedItems[tag];
        var currentButton = ItemSelectionWindow.GetButton<UItemOption>(
            ItemSelectionWindow.DesiredFocusIndex
        );
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

    public void ApplyItemFilter(Func<FItemHandle, bool> filter)
    {
        ItemSelectionWindow.ApplyItemFilter(filter);
    }

    public void ResetItemFilter()
    {
        ItemSelectionWindow.ResetItemFilter();
    }

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

    [UFunction(FunctionFlags.BlueprintCallable, Category = "Items|Selection")]
    public void Refresh()
    {
        ItemSelectionWindow.Refresh();
    }

    private void SetHoveredItem(FItemHandle item, int quantity)
    {
        CurrentItem = item;
        Bag.SetLastViewedIndex(
            ItemSelectionWindow.CurrentPocket,
            ItemSelectionWindow.DesiredFocusIndex
        );

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

    protected override UWidget? BP_GetDesiredFocusTarget()
    {
        return FocusUtilities.GetFirstActivatedWidget(CommandWidget, ItemSelectionWindow);
    }

    // TODO: Using item events
}
