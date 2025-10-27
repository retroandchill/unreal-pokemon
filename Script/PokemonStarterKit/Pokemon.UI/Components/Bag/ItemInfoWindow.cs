using Pokemon.Data.Model.PBS;
using Pokemon.UI.Components.Common;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.Attributes.MetaTags;
using UnrealSharp.CommonUI;
using UnrealSharp.UMG;

namespace Pokemon.UI.Components.Bag;

/// <summary>
/// The UItemInfoWindow class is responsible for displaying item information in the bag UI.
/// It provides the item name and description text fields, and manages their visibility and content.
/// This class derives from UItemInfoWidgetBase and extends its functionality to specifically
/// update UI elements with item data.
/// </summary>
[UClass(ClassFlags.Abstract)]
public partial class UItemInfoWindow : UItemInfoWidgetBase
{
    [UProperty]
    [BindWidget]
    private partial UCommonTextBlock ItemNameText { get; }

    [UProperty]
    [BindWidget]
    private partial UCommonTextBlock ItemDescriptionText { get; }

    /// <inheritdoc />
    protected override void OnItemSet_Implementation(FItemHandle item)
    {
        ItemNameText.Visibility = ESlateVisibility.SelfHitTestInvisible;
        ItemDescriptionText.Visibility = ESlateVisibility.SelfHitTestInvisible;

        ItemNameText.Text = item.Entry.Name;
        ItemDescriptionText.Text = item.Entry.Description;
    }
}
