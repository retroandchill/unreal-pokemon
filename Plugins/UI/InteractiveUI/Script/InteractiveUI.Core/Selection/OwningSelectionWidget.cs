using UnrealSharp.Attributes;
using UnrealSharp.CommonUI;
using UnrealSharp.UMG;

namespace InteractiveUI.Core.Selection;

[UClass]
public class UOwningSelectionWidget : USelectionWidget
{
    public override void Construct()
    {
        base.Construct();
        Buttons.OnButtonAdded += SlotButton;
        Buttons.OnButtonRemoved += b => b.RemoveFromParent();
    }

    [UFunction(FunctionFlags.BlueprintEvent, Category = "Selection")]
    protected virtual void SlotButton(int index, UWidget button)
    {
        // No native implementation
    }
}
