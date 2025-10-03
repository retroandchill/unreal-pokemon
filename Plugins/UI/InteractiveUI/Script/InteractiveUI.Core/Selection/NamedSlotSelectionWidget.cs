using UnrealSharp.Attributes;
using UnrealSharp.Attributes.MetaTags;
using UnrealSharp.CommonUI;
using UnrealSharp.Engine;
using UnrealSharp.UMG;

namespace InteractiveUI.Core.Selection;

/// <summary>
/// Represents a widget that manages the selection of named slots. This widget is an abstract class
/// that extends the functionality provided by the base USelectionWidget. It ensures the available
/// buttons in the selection group are updated based on the current content and conditions. Implements
/// the IRefreshable interface for updating its state.
/// </summary>
[UClass(ClassFlags.Abstract)]
public class UNamedSlotSelectionWidget : USelectionWidget, IRefreshable
{
    /// <summary>
    /// Gets the associated content slot for this widget. The content slot typically holds the panel widget
    /// containing child elements that can dynamically interact with the selection widget.
    /// </summary>
    /// <remarks>
    /// The ContentSlot is primarily used to access and manage child widgets within a panel context.
    /// It enables actions like updating available options or interacting with child widgets.
    /// </remarks>
    [UProperty]
    [BindWidget]
    private UNamedSlot ContentSlot { get; }

    /// <inheritdoc />
    public override void Construct()
    {
        base.Construct();
        UpdateOptions();
    }

    /// <summary>
    /// Updates the available selection options in the widget by processing the children of the assigned content slot.
    /// Verifies that the content is a valid panel widget and iterates through its child elements to identify buttons.
    /// If a child element implements the IConditionalWidget interface, updates its enabled state and excludes disabled widgets.
    /// Adds valid buttons to the Buttons group and ensures the desired focus index is within an acceptable range.
    /// Intended to be used for refreshing the selection state based on the content of the widget.
    /// </summary>
    [UFunction(FunctionFlags.BlueprintCallable, Category = "Selection")]
    public void UpdateOptions()
    {
#if WITH_EDITOR
        if (!SystemLibrary.IsValid(ContentSlot))
            return;
#endif

        if (ContentSlot.Content is not UPanelWidget contentWidget)
        {
            LogInteractiveUICore.LogError("ContentSlot contain a UPanelWidget");
            return;
        }

        Buttons.RemoveAll();
        for (var i = 0; i < contentWidget.ChildrenCount; i++)
        {
            var child = contentWidget.GetChildAt(i);
            if (child is not UCommonButtonBase button)
            {
                continue;
            }

            if (child is IConditionalWidget conditionalWidget)
            {
                conditionalWidget.UpdateEnabled();
                if (!conditionalWidget.IsWidgetEnabled)
                    continue;
            }

            Buttons.AddWidget(button);
        }

        DesiredFocusIndex = Math.Clamp(DesiredFocusIndex, 0, Buttons.ButtonCount - 1);
    }

    /// <inheritdoc />
    public void Refresh()
    {
        UpdateOptions();
    }
}
