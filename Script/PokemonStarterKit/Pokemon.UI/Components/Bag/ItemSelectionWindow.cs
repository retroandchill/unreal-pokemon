using InteractiveUI.Core.Selection;
using Pokemon.Core.Entities;
using Pokemon.Data.Model.PBS;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.GameplayTags;
using UnrealSharp.InteractiveUI;

namespace Pokemon.UI.Components.Bag;

[UClass(ClassFlags.Abstract)]
public class UItemSelectionWindow : UOwningSelectionWidget
{
    [UProperty(PropertyFlags.EditAnywhere, Category = "Display")]
    private TSubclassOf<UItemOption> ItemEntryClass { get; }

    public event Action<FGameplayTag>? OnPocketChanged;

    public event Action<FItemHandle, int>? OnItemHovered;

    public event Action<FItemHandle, int>? OnItemSelected;

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
        }
    }

    public override void Construct()
    {
        base.Construct();
        Buttons.OnHoveredButtonBaseChanged += [UFunction] (_, i) => OnSelectionChange(i);
        Buttons.OnButtonBaseClicked += [UFunction] (_, i) => ProcessConfirm(i);
    }

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

    private void OnSelectionChange(int index)
    {
        Bag.SetLastViewedIndex(CurrentPocket, index);
        if (Buttons.GetButtonBaseAtIndex(index) is UItemOption option)
        {
            OnItemHovered?.Invoke(option.Item, option.Quantity);
        }
        else
        {
            OnItemHovered?.Invoke(default, 0);
        }
    }

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

        DesiredFocusIndex =
            Buttons.ButtonCount > 0 ? Math.Clamp(DesiredFocusIndex, 0, Buttons.ButtonCount - 1) : 0;

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
