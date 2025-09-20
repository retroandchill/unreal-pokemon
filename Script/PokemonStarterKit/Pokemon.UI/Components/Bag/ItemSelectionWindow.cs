using InteractiveUI.Core.Selection;
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
public class UItemSelectionWindow : UOwningSelectionWidget
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

    [UFunction(FunctionFlags.BlueprintCallable, Category = "Items|Selection")]
    private void OnSelectionChange(int index)
    {
        Bag.SetLastViewedIndex(CurrentPocket, index);
        if (Buttons.GetButtonBaseAtIndex(index) is UItemOption option)
        {
            OnItemChanged.Invoke(option.Item, option.Quantity);
        }
        else
        {
            OnItemChanged.Invoke(default, 0);
        }
    }

    protected void ProcessConfirm(int currentIndex)
    {
        if (Buttons.GetButtonBaseAtIndex(currentIndex) is not UItemOption option)
            throw new InvalidOperationException("No option at index");
        OnItemSelected.Invoke(option.Item, option.Quantity);
    }

    protected override bool OnHandleBackAction()
    {
        OnNoItemSelected.Invoke();
        return true;
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
        DesiredFocusIndex = Math.Clamp(DesiredFocusIndex, 0, Buttons.ButtonCount - 1);
        OnPocketChanged.Invoke(CurrentPocket);
    }
}
