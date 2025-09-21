using InteractiveUI.Core;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.CommonUI;

namespace Pokemon.UI.Components.Common;

[UClass(ClassFlags.Abstract)]
public class UItemQuantityDisplayBase : UCommonUserWidget, IRefreshable
{
    [UProperty(PropertyFlags.BlueprintReadWrite, Category = "Content")]
    public int Quantity
    {
        get;
        set
        {
            ArgumentOutOfRangeException.ThrowIfLessThan(value, 0);
            field = value;
            Refresh();
        }
    }

    [UProperty(PropertyFlags.EditAnywhere | PropertyFlags.BlueprintReadWrite, Category = "Content")]
    public TSubclassOf<UCommonTextStyle> TextStyle
    {
        get;
        set
        {
            if (field == value)
                return;

            field = value;
            OnTextStyleChanged();
        }
    }

    public virtual void Refresh()
    {
        // No implementation here
    }

    [UFunction(FunctionFlags.BlueprintEvent, Category = "Content")]
    protected virtual void OnTextStyleChanged()
    {
        // No implementation here
    }
}
