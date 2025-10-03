using UnrealSharp.Attributes;
using UnrealSharp.CommonUI;
using UnrealSharp.UnrealSharpCommonUI;

namespace InteractiveUI.Core.Selection;

/// A custom implementation of a button group used for managing a collection of selectable buttons.
/// This class extends UCSCommonButtonGroupBase to provide specific behavior for handling button addition and removal events.
[UClass]
public class USelectableButtonGroup : UCSCommonButtonGroupBase
{
    /// <summary>
    /// An event that is triggered whenever a new button is added to the button group.
    /// </summary>
    /// <remarks>
    /// The event provides two parameters:
    /// - An integer representing the index of the added button.
    /// - A reference to the instance of the button being added, of type <see cref="UCommonButtonBase"/>.
    /// </remarks>
    /// <example>
    /// This event can be used to handle custom logic when a button is added to a <see cref="USelectableButtonGroup"/>.
    /// </example>
    public event Action<int, UCommonButtonBase>? OnButtonAdded;

    /// <summary>
    /// An event that is triggered whenever a button is removed from the button group.
    /// </summary>
    /// <remarks>
    /// The event provides a single parameter:
    /// - A reference to the instance of the button being removed, of type <see cref="UCommonButtonBase"/>.
    /// </remarks>
    /// <example>
    /// This event can be used to handle custom logic when a button is removed from a <see cref="USelectableButtonGroup"/>.
    /// </example>
    public event Action<UCommonButtonBase>? OnButtonRemoved;

    /// <inheritdoc />
    protected override void OnWidgetAdded(UCommonButtonBase button)
    {
        OnButtonAdded?.Invoke(ButtonCount - 1, button);
    }

    /// <inheritdoc />
    protected override void OnWidgetRemoved(UCommonButtonBase button)
    {
        OnButtonRemoved?.Invoke(button);
    }
}
