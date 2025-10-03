using InteractiveUI.Core;
using Pokemon.Core;
using Pokemon.Data.Model.PBS;
using UnrealSharp.Attributes;
using UnrealSharp.Attributes.MetaTags;
using UnrealSharp.CommonUI;

namespace Pokemon.UI.Components.Common;

/// <summary>
/// UItemQuantityDisplay is a UI component responsible for displaying
/// the quantity of an item, inheriting its functionalities from
/// UItemQuantityDisplayBase. This class updates its display to
/// reflect changes in item quantity and adjusts the text style dynamically.
/// </summary>
[UClass(ClassFlags.Abstract)]
public class UItemQuantityDisplay : UItemQuantityDisplayBase
{
    /// <summary>
    /// Gets or sets the numeric text block responsible for displaying
    /// the item quantity in the UI. This property binds to the UI
    /// element dynamically and updates its text value according to
    /// the current item quantity. Additionally, it reflects changes
    /// in text styles when triggered.
    /// </summary>
    [UProperty]
    [BindWidget]
    public UCommonNumericTextBlock QuantityText { get; }

    /// <inheritdoc />
    public override void Refresh()
    {
        QuantityText.CurrentValue = Quantity;
    }

    /// <inheritdoc />
    protected override void OnTextStyleChanged()
    {
        QuantityText.Style = TextStyle;
    }
}
