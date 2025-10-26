using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.Core;
using UnrealSharp.UnrealSharpCommonUI;

namespace InteractiveUI.Core.Selection;

/// <summary>
/// Represents a widget for selecting an option from a list of predefined choices.
/// This class extends the functionality of UOwningSelectionWidget and provides
/// additional features for handling option-based selection UI widgets.
/// </summary>
[UClass(ClassFlags.Abstract)]
public partial class UOptionSelectionWidget : UOwningSelectionWidget
{
    /// <summary>
    /// Specifies the class type of button widgets used within the selection widget.
    /// This is a subclass of USimpleTextButtonBase and determines the visual
    /// and functional attributes of buttons created for each selectable option.
    /// </summary>
    [UProperty(PropertyFlags.EditAnywhere, Category = "Options")]
    public partial TSubclassOf<USimpleTextButtonBase> ButtonClass { get; }

    /// <summary>
    /// Contains the list of displayable text options available for selection in the widget.
    /// These options are used to populate the interactive UI elements displayed to the user.
    /// </summary>
    [UProperty(PropertyFlags.EditAnywhere, Category = "Options")]
    public partial TArray<FText> Options { get; }

    /// <summary>
    /// Determines the index of the cancel option within the list of selectable options.
    /// This property is nullable and indicates the option that corresponds to a cancel action,
    /// if such an option is defined. If no cancel option is specified or the index is invalid,
    /// this property will be null.
    /// </summary>
    [UProperty(PropertyFlags.EditAnywhere, Category = "Options")]
    public partial int? CancelOptionIndex { get; private set; }

    /// <summary>
    /// Occurs when an option is selected from the available choices in the `UOptionSelectionWidget`.
    /// The event is triggered with the index of the selected option as its parameter.
    /// </summary>
    public event Action<int>? OnOptionSelected;

    /// <inheritdoc />
    protected override void Construct_Implementation()
    {
        base.Construct_Implementation();
        Buttons.OnButtonBaseClicked += [UFunction] (_, i) => OnOptionSelected?.Invoke(i);
    }

    /// <summary>
    /// Configures the widget with a list of options and an optional cancel option index.
    /// </summary>
    /// <param name="options">The collection of options to display in the widget.</param>
    /// <param name="cancelOptionIndex">
    /// An optional index of the option that acts as a cancel choice.
    /// If the value is not within the range of the provided options, it is ignored.
    /// </param>
    public void SetOptions(IEnumerable<FText> options, int? cancelOptionIndex = null)
    {
        Options.Clear();
        foreach (var option in options)
        {
            Options.Add(option);
        }

        CancelOptionIndex =
            cancelOptionIndex is >= 0 && cancelOptionIndex.Value < Options.Count ? cancelOptionIndex.Value : null;
    }

    private void CreateOptions()
    {
        Buttons.RemoveAll();
        foreach (var (i, option) in Options.Index())
        {
            var newButton = WidgetTree.ConstructWidget(ButtonClass);
            newButton.Text = option;

            if (i == CancelOptionIndex)
            {
                newButton.TriggeringEnhancedInputAction = UCommonInputExtensions.EnhancedInputBackAction;
            }

            Buttons.AddWidget(newButton);
        }
    }
}
