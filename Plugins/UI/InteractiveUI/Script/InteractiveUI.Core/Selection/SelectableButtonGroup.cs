using UnrealSharp.Attributes;
using UnrealSharp.CommonUI;
using UnrealSharp.UnrealSharpCommonUI;

namespace InteractiveUI.Core.Selection;

[UClass]
public class USelectableButtonGroup : UCSCommonButtonGroupBase
{
    public event Action<int, UCommonButtonBase>? OnButtonAdded;

    public event Action<UCommonButtonBase>? OnButtonRemoved;

    protected override void OnWidgetAdded(UCommonButtonBase button)
    {
        OnButtonAdded?.Invoke(ButtonCount - 1, button);
    }

    protected override void OnWidgetRemoved(UCommonButtonBase button)
    {
        OnButtonRemoved?.Invoke(button);
    }
}
