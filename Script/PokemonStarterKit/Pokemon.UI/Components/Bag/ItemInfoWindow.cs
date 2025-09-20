using Pokemon.Data.Model.PBS;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.Attributes.MetaTags;
using UnrealSharp.CommonUI;

namespace Pokemon.UI.Components.Bag;

[UClass(ClassFlags.Abstract)]
public class UItemInfoWindow : UCommonUserWidget
{
    [UProperty]
    [BindWidget]
    private UCommonTextBlock ItemNameText { get; }

    [UProperty]
    [BindWidget]
    private UCommonTextBlock ItemDescriptionText { get; }

    [UFunction(
        FunctionFlags.BlueprintCallable | FunctionFlags.BlueprintEvent,
        Category = "Display"
    )]
    public virtual void SetItem(FItemHandle item, int quantity)
    {
        ItemNameText.Text = item.Entry.Name;
        ItemDescriptionText.Text = item.Entry.Description;
    }

    [UFunction(
        FunctionFlags.BlueprintCallable | FunctionFlags.BlueprintEvent,
        Category = "Display"
    )]
    public virtual void ClearItem()
    {
        ItemNameText.Text = FText.None;
        ItemDescriptionText.Text = FText.None;
    }
}
