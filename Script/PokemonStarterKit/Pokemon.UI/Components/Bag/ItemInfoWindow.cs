using Pokemon.Data.Model.PBS;
using Pokemon.UI.Components.Common;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.Attributes.MetaTags;
using UnrealSharp.CommonUI;
using UnrealSharp.UMG;

namespace Pokemon.UI.Components.Bag;

[UClass(ClassFlags.Abstract)]
public class UItemInfoWindow : UItemInfoWidgetBase
{
    [UProperty]
    [BindWidget]
    private UCommonTextBlock ItemNameText { get; }

    [UProperty]
    [BindWidget]
    private UCommonTextBlock ItemDescriptionText { get; }

    protected override void OnItemSet(FItemHandle item)
    {
        ItemNameText.Visibility = ESlateVisibility.SelfHitTestInvisible;
        ItemDescriptionText.Visibility = ESlateVisibility.SelfHitTestInvisible;

        ItemNameText.Text = item.Entry.Name;
        ItemDescriptionText.Text = item.Entry.Description;
    }
}
