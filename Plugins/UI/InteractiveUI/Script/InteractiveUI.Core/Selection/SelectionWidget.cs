using UnrealSharp.Attributes;
using UnrealSharp.Attributes.MetaTags;
using UnrealSharp.CommonUI;
using UnrealSharp.Engine;
using UnrealSharp.UMG;

namespace InteractiveUI.Core.Selection;

[UClass(ClassFlags.Abstract)]
public class USelectionWidget : UCommonActivatableWidget
{
    [UProperty(PropertyFlags.Transient)]
    protected USelectableButtonGroup Buttons { get; private set; }
    
    public event Action? OnBackAction;

    [UProperty(PropertyFlags.EditAnywhere, Category = "Selection")]
    [UIMin("0")]
    [ClampMin("0")]
    public int DesiredFocusIndex
    {
        get;
        set
        {
            if (Buttons.ButtonCount == 0)
            {
                field = 0;
                return;
            }

            field = Math.Clamp(value, 0, Buttons.ButtonCount - 1);
        }
    }

    public override void PreConstruct(bool isDesignTime)
    {
        if (SystemLibrary.IsValid(Buttons))
        {
            Buttons.RemoveAll();
        }

        Buttons = NewObject<USelectableButtonGroup>(this);
    }

    public override void Construct()
    {
        Buttons.OnButtonBaseClicked += [UFunction] (_, i) => DesiredFocusIndex = i;
    }

    protected override UWidget? BP_GetDesiredFocusTarget()
    {
        return Buttons.GetButtonBaseAtIndex(DesiredFocusIndex);
    }

    protected override bool OnHandleBackAction()
    {
        OnBackAction?.Invoke();
        return true;
    }
}
