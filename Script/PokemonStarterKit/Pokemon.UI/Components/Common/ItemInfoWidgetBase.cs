using InteractiveUI.Core;
using Pokemon.Data.Model.PBS;
using UnrealSharp.Attributes;
using UnrealSharp.CommonUI;

namespace Pokemon.UI.Components.Common;

[UClass(ClassFlags.Abstract)]
public class UItemInfoWidgetBase : UCommonUserWidget, IRefreshable
{
    [UProperty(PropertyFlags.BlueprintReadWrite, Category = "Content")]
    public FItemHandle Item
    {
        get;
        set
        {
            if (field == value)
                return;

            field = value;
            Refresh();
        }
    }

    public void Refresh()
    {
        if (Item.IsValid)
        {
            OnItemSet(Item);
        }
    }

    [UFunction(FunctionFlags.BlueprintEvent, Category = "Display")]
    protected virtual void OnItemSet(FItemHandle item) { }
}
