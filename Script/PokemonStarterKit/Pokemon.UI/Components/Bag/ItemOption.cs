using Pokemon.Data.Model.PBS;
using Pokemon.UI.Components.Common;
using UnrealSharp.Attributes;
using UnrealSharp.Attributes.MetaTags;
using UnrealSharp.CommonUI;
using UnrealSharp.UMG;

namespace Pokemon.UI.Components.Bag;

[UClass(ClassFlags.Abstract)]
public class UItemOption : UCommonButtonBase
{
    [UProperty(PropertyFlags.BlueprintReadOnly, Category = "Inventory")]
    public FItemHandle Item { get; private set; }

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

    [UFunction(FunctionFlags.BlueprintCallable, Category = "Inventory")]
    public void SetItem(FItemHandle item, int quantity)
    {
        Item = item;
        Quantity = quantity;
        OnItemSet(item, quantity);
    }

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

    protected override void OnCurrentTextStyleChanged()
    {
        var currentStyle = CurrentTextStyleClass;
        ItemNameText.Style = currentStyle;
        ItemQuantityText.TextStyle = currentStyle;
    }
}
