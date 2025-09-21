using InteractiveUI.Core;
using Pokemon.Core;
using Pokemon.Data.Model.PBS;
using UnrealSharp.Attributes;
using UnrealSharp.Attributes.MetaTags;
using UnrealSharp.CommonUI;

namespace Pokemon.UI.Components.Common;

[UClass(ClassFlags.Abstract)]
public class UItemQuantityDisplay : UItemQuantityDisplayBase
{
    [UProperty]
    [BindWidget]
    public UCommonNumericTextBlock QuantityText { get; }

    public override void Refresh()
    {
        QuantityText.CurrentValue = Quantity;
    }

    protected override void OnTextStyleChanged()
    {
        QuantityText.Style = TextStyle;
    }
}
