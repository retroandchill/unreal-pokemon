using InteractiveUI.Core.Utilities;
using UnrealSharp.Attributes;
using UnrealSharp.Attributes.MetaTags;
using UnrealSharp.CommonUI;
using UnrealSharp.Engine;
using UnrealSharp.UMG;

namespace InteractiveUI.Core.Selection;

/// <summary>
/// USelectionWidget is an abstract class derived from UCommonActivatableWidget. It provides
/// functionality for managing and interacting with a group of selectable buttons in a UI context.
/// </summary>
[UClass(ClassFlags.Abstract)]
public partial class USelectionWidget : UCommonActivatableWidget
{
    /// <summary>
    /// Represents a group of selectable buttons within a selection widget.
    /// This property is used to manage button interactions, button counts,
    /// and focus-related operations within the widget.
    /// </summary>
    /// <remarks>
    /// The Buttons property is a protected, transient member of type
    /// <see cref="USelectableButtonGroup"/> and is primarily used internally
    /// for managing the selection logic. External interactions with the buttons
    /// are typically performed using public methods defined in
    /// <see cref="USelectionWidget"/>.
    /// </remarks>
    [UProperty(PropertyFlags.Transient)]
    protected USelectableButtonGroup Buttons { get; private set; }

    /// <summary>
    /// Represents an event triggered when a "back" action is invoked within the widget.
    /// This event is typically used to handle user input indicating a desire to navigate
    /// away from the current screen or cancel the current operation.
    /// </summary>
    /// <remarks>
    /// The OnBackAction event is integral to the navigation and interaction flow of the associated widget.
    /// It is commonly subscribed to for implementing back navigation functionality or related logic
    /// within derived classes.
    /// This event is invoked internally by the <see cref="USelectionWidget.OnHandleBackAction"/> method and
    /// may be utilized by other classes, such as screens or components, to appropriately handle the back action.
    /// </remarks>
    public event Action? OnBackAction;

    /// <summary>
    /// Represents the desired index of the button that should have focus within the selection widget.
    /// This property ensures that the focus index is maintained within the valid range of buttons
    /// present in the widget.
    /// </summary>
    /// <remarks>
    /// The DesiredFocusIndex property is an integer value used to indicate the zero-based index
    /// of the button that is intended to be focused. It automatically clamps its value to
    /// ensure it does not exceed the range defined by the current number of buttons.
    /// Any attempts to set an invalid value will result in the property being adjusted to
    /// fall within the allowed range.
    /// </remarks>
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

    /// <summary>
    /// Retrieves the total number of selectable buttons within the current selection widget.
    /// </summary>
    /// <remarks>
    /// This property provides a read-only count of the buttons managed by the widget's internal
    /// <see cref="USelectableButtonGroup"/>. It reflects the total number of buttons available
    /// for interaction within the selection widget's scope. This value is derived from the
    /// internal button group and is primarily used for scenarios such as UI logic validation
    /// or layout adjustments.
    /// </remarks>
    public int ButtonCount
    {
        [UFunction(FunctionFlags.BlueprintPure, Category = "Selection")]
        get => Buttons.ButtonCount;
    }

    /// <inheritdoc />
    protected override void PreConstruct_Implementation(bool isDesignTime)
    {
        if (SystemLibrary.IsValid(Buttons))
        {
            Buttons.RemoveAll();
        }

        Buttons = NewObject<USelectableButtonGroup>(this);
    }

    /// <inheritdoc />
    protected override void Construct_Implementation()
    {
        Buttons.OnButtonBaseClicked += ChangeDesiredFocusIndex;
        Buttons.OnHoveredButtonBaseChanged += ChangeDesiredFocusIndex;
    }

    /// <summary>
    /// Retrieves the button at the specified index in the selectable button group, if available.
    /// </summary>
    /// <param name="index">The zero-based index of the button to retrieve.</param>
    /// <returns>A <see cref="UCommonButtonBase"/> representing the button at the specified index, or null if the button does not exist.</returns>
    public UCommonButtonBase? GetButton(int index)
    {
        return Buttons.GetButtonBaseAtIndex(index);
    }

    /// <summary>
    /// Retrieves a button of the specified type T at the given index from the button group.
    /// </summary>
    /// <typeparam name="T">The type of button to retrieve. Must inherit from UCommonButtonBase.</typeparam>
    /// <param name="index">The index of the button in the group to retrieve.</param>
    /// <returns>
    /// The button of type T if found at the specified index; otherwise, null if the button at the index is
    /// not of type T or does not exist.
    /// </returns>
    public T? GetButton<T>(int index)
        where T : UCommonButtonBase
    {
        return Buttons.GetButton<T>(index);
    }

    public UCommonButtonBase GetRequiredButton(int index)
    {
        return Buttons.GetButtonBaseAtIndex(index) ?? throw new InvalidOperationException("No button at index");
    }

    /// <summary>
    /// Retrieves a button of type <typeparamref name="T"/> at the specified index from the button group.
    /// This method guarantees a non-null result and throws an exception if the button is not found.
    /// </summary>
    /// <typeparam name="T">The type of button to retrieve. Must inherit from <see cref="UCommonButtonBase"/>.</typeparam>
    /// <param name="index">The zero-based index of the button to retrieve.</param>
    /// <returns>The button of type <typeparamref name="T"/> at the specified index.</returns>
    /// <exception cref="InvalidOperationException">Thrown when the button at the specified index is not of the expected type or is null.</exception>
    public T GetRequiredButton<T>(int index)
        where T : UCommonButtonBase
    {
        return Buttons.GetRequiredButton<T>(index);
    }

    [UFunction]
    private void ChangeDesiredFocusIndex(UCommonButtonBase button, int index)
    {
        DesiredFocusIndex = index;
    }

    /// <inheritdoc />
    protected override UWidget? BP_GetDesiredFocusTarget_Implementation()
    {
        return Buttons.GetButtonBaseAtIndex(DesiredFocusIndex);
    }

    /// <inheritdoc />
    protected override bool OnHandleBackAction_Implementation()
    {
        OnBackAction?.Invoke();
        return true;
    }
}
