using UnrealSharp.Attributes;
using UnrealSharp.CommonUI;
using UnrealSharp.UMG;

namespace InteractiveUI.Core.Selection;

/// <summary>
/// Represents a UI component that manages a selection of buttons owned by the widget itself.
/// This class extends the functionality of USelectionWidget by providing additional customization options
/// and a customizable mechanism for handling button addition and removal.
/// </summary>
[UClass]
public partial class UOwningSelectionWidget : USelectionWidget
{
    /// <inheritdoc />
    protected override void Construct_Implementation()
    {
        base.Construct();
        Buttons.OnButtonAdded += SlotButton;
        Buttons.OnButtonRemoved += b => b.RemoveFromParent();
    }

    /// <summary>
    /// Called when a button is added to the selection widget. This method provides functionality
    /// to modify or position the button within the user interface based on the provided index.
    /// </summary>
    /// <param name="index">The index at which the button is added.</param>
    /// <param name="button">The button (widget) that has been added.</param>
    [UFunction(FunctionFlags.BlueprintEvent, Category = "Selection")]
    protected partial void SlotButton(int index, UWidget button);

    protected partial void SlotButton_Implementation(int index, UWidget button)
    {
        // Nothing to do here.
    }
}
