using Pokemon.Core.Entities;
using Pokemon.Data.Model.PBS;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.GameplayTags;
using UnrealSharp.InteractiveUI;

namespace Pokemon.UI.Components.Bag;

[UMultiDelegate]
public delegate void OnPocketChanged(FGameplayTag pocket);

[UMultiDelegate]
public delegate void OnItemChanged(FItemHandle item, int quantity);

[UMultiDelegate]
public delegate void OnNoItemSelected();

[UClass(ClassFlags.Abstract)]
public class UItemSelectionWindow : USelectableWidget
{
    [UProperty(PropertyFlags.EditAnywhere, Category = "Display")]
    private TSubclassOf<UItemOption> ItemEntryClass { get; }

    [UProperty(PropertyFlags.BlueprintAssignable, Category = "Events")]
    public TMulticastDelegate<OnItemChanged> OnItemSelected { get; set; }

    [UProperty(PropertyFlags.BlueprintAssignable, Category = "Events")]
    public TMulticastDelegate<OnNoItemSelected> OnNoItemSelected { get; set; }

    [UProperty(PropertyFlags.BlueprintAssignable, Category = "Events")]
    public TMulticastDelegate<OnPocketChanged> OnPocketChanged { get; set; }

    [UProperty(PropertyFlags.BlueprintAssignable, Category = "Events")]
    public TMulticastDelegate<OnItemChanged> OnItemChanged { get; set; }

    private Func<FItemHandle, bool>? _itemFilter;

    [UProperty]
    public UPokemonBag Bag { get; set; }

    public FGameplayTag CurrentPocket
    {
        get;
        set
        {
            field = value;
            UpdatePocket();

            DesiredFocusTarget?.SetFocus();
        }
    }

    public FItemHandle CurrentItem => GetSelectableOption<UItemOption>(Index)?.Item ?? default;

    public int ItemQuantity => GetSelectableOption<UItemOption>(Index)?.Quantity ?? 0;

    public void Refresh()
    {
        UpdatePocket();
    }

    public void ApplyItemFilter(Func<FItemHandle, bool> filter)
    {
        _itemFilter = filter;
        UpdatePocket();
    }

    public void ResetItemFilter()
    {
        _itemFilter = null;
        UpdatePocket();
    }

    protected override void OnSelectionChange(int oldIndex, int newIndex)
    {
        base.OnSelectionChange(oldIndex, newIndex);
        Bag.SetLastViewedIndex(CurrentPocket, newIndex);
        var option = GetSelectableOption<UItemOption>(newIndex);

        if (option is not null)
        {
            OnItemChanged.Invoke(option.Item, option.Quantity);
        }
        else
        {
            OnItemChanged.Invoke(default, 0);
        }
    }

    protected override void ProcessConfirm(int currentIndex)
    {
        var option = GetSelectableOption<UItemOption>(currentIndex);
        if (option is null)
            throw new InvalidOperationException("No option at index");
        OnItemSelected.Invoke(option.Item, option.Quantity);
    }

    protected override void ProcessCancel()
    {
        OnNoItemSelected.Invoke();
    }

    private void UpdatePocket()
    {
        ClearSelectableOptions();
        foreach (var (item, quantity) in Bag.AllItems[CurrentPocket])
        {
            AddItemToWindow(item, quantity);
        }
        Index = Math.Clamp(0, ItemCount - 1, Index);
        OnPocketChanged.Invoke(CurrentPocket);
    }

    private void AddItemToWindow(FItemHandle item, int quantity)
    {
        if (_itemFilter is not null && !_itemFilter(item))
            return;

        var option = WidgetTree.ConstructWidget(ItemEntryClass);
        option.SetItem(item, quantity);
        SlotOption(option);
    }
}
