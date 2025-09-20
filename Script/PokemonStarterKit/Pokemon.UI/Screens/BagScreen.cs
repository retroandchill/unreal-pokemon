using InteractiveUI.Core;
using Pokemon.Core;
using Pokemon.Core.Services.Async;
using Pokemon.Data.Model.PBS;
using Pokemon.UI.Components.Bag;
using UnrealSharp.Attributes;
using UnrealSharp.Attributes.MetaTags;
using UnrealSharp.CommonUI;
using UnrealSharp.CoreUObject;
using UnrealSharp.Engine;
using UnrealSharp.InteractiveUI;

namespace Pokemon.UI.Screens;

[UClass(ClassFlags.Abstract)]
public class UBagScreen : UCommonActivatableWidget
{
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
        var bag = GetGameInstanceSubsystem<UPokemonSubsystem>().Bag;
        ItemSelectionWindow.Bag = bag;
        PocketTabWidget.ItemSelectionWindow = ItemSelectionWindow;
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

    [UFunction(FunctionFlags.BlueprintCallable, Category = "Items|Selection")]
    protected void SelectItem(FItemHandle itemHandle, int quantity)
    {
        if (_onItemSelected is not null)
        {
            _onItemSelected(new FItemSlotInfo(itemHandle, quantity));
            return;
        }

        ToggleItemSelection(false);
        ShowItemCommands();
    }

    [UFunction(FunctionFlags.BlueprintCallable, Category = "Items|Selection")]
    protected void OnItemSelectionCanceled()
    {
        if (_onItemSelected is not null)
        {
            _onItemSelected(null);
            return;
        }

        this.PopContentFromLayer();
    }

    [UFunction(FunctionFlags.BlueprintEvent, Category = "Items|Selection")]
    protected virtual void ShowItemCommands()
    {
        // No native implementation
    }

    // TODO: Using item events
}
