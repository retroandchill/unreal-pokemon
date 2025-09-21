using InteractiveUI.Core.Utilities;
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

    [UProperty(PropertyFlags.EditAnywhere | PropertyFlags.BlueprintReadWrite, Category = "Selection")]
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

    public int ButtonCount
    {
        [UFunction(FunctionFlags.BlueprintPure, Category = "Selection")]
        get => Buttons.ButtonCount;
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
        Buttons.OnButtonBaseClicked += ChangeDesiredFocusIndex;
        Buttons.OnHoveredButtonBaseChanged += ChangeDesiredFocusIndex;
    }

    public UCommonButtonBase? GetButton(int index)
    {
        return Buttons.GetButtonBaseAtIndex(index);   
    }

    public T? GetButton<T>(int index) where T : UCommonButtonBase
    {
        return Buttons.GetButton<T>(index);  
    }
    
    public UCommonButtonBase GetRequiredButton(int index)
    {
        return Buttons.GetButtonBaseAtIndex(index) ?? throw new InvalidOperationException("No button at index"); 
    }

    public T GetRequiredButton<T>(int index) where T : UCommonButtonBase
    {
        return Buttons.GetRequiredButton<T>(index);  
    }

    [UFunction]
    private void ChangeDesiredFocusIndex(UCommonButtonBase button, int index)
    {
        DesiredFocusIndex = index;
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
