using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.CommonInput;
using UnrealSharp.CommonUI;

namespace InteractiveUI.Core.Actions;

/// <summary>
/// Represents an interactive action button that inherits from UCommonBoundActionButton.
/// </summary>
/// <remarks>
/// UInteractiveActionButton extends the functionalities of UCommonBoundActionButton to provide
/// behavior triggered by input method changes, such as keyboard or gamepad.
/// </remarks>
[UClass(ClassFlags.Abstract)]
[UMetaData("DisableNativeTick")]
public partial class UInteractiveActionButton : UCommonBoundActionButton
{
    /// <summary>
    /// Defines the style to be applied to the button when the input method is a keyboard or mouse.
    /// </summary>
    /// <remarks>
    /// This property determines the visual appearance and interaction styling of the button
    /// when the associated input method is `MouseAndKeyboard`. This aligns with the customization
    /// of user interaction for devices operating with keyboard and mouse setup.
    /// The assigned style should be a class derived from <see cref="UCommonButtonStyle"/>,
    /// which provides detailed button appearance configurations such as brushes and sizes.
    /// This property is typically used in conjunction with other input-specific styles such as
    /// `GamepadStyle` and `TouchStyle` within the `HandleInputMethodChanged` logic to dynamically
    /// update the button's appearance based on the active input method.
    /// </remarks>
    /// <type>UCommonButtonStyle</type>
    [UProperty(PropertyFlags.EditAnywhere, Category = "Style")]
    private TSubclassOf<UCommonButtonStyle> KeyboardStyle { get; }

    /// <summary>
    /// Defines the style to be applied to the button when the input method is a gamepad.
    /// </summary>
    /// <remarks>
    /// This property specifies the visual design and interaction behavior for buttons
    /// when the input device in use is a gamepad. It allows for the customization of the button's appearance
    /// to provide a cohesive user interface when interacting with gamepad controls.
    /// The value assigned to this property should be a class derived from <see cref="UCommonButtonStyle"/>,
    /// which configures properties like size, textures, and states such as pressed or disabled.
    /// Typically used in `HandleInputMethodChanged` logic alongside other input-specific styles such as
    /// `KeyboardStyle` and `TouchStyle` to seamlessly adapt the button style to the active input method.
    /// </remarks>
    /// <type>UCommonButtonStyle</type>
    [UProperty(PropertyFlags.EditAnywhere, Category = "Style")]
    private TSubclassOf<UCommonButtonStyle> GamepadStyle { get; }

    /// <summary>
    /// Defines the style to be applied to the button when the input method is touch.
    /// </summary>
    /// <remarks>
    /// This property determines the visual appearance and interaction styling of the button
    /// when the associated input method is `Touch`. It is utilized to align the user interface with
    /// touch-specific interaction principles, ensuring an optimal experience for touch-based inputs.
    /// The assigned style must be a class derived from <see cref="UCommonButtonStyle"/>, which specifies
    /// appearance attributes like brushes, dimensions, and interaction states for the button.
    /// Typically, this property is used in scenarios where input method changes invoke an
    /// update in the button's appearance, such as through logic in `HandleInputMethodChanged`.
    /// </remarks>
    /// <type>UCommonButtonStyle</type>
    [UProperty(PropertyFlags.EditAnywhere, Category = "Style")]
    private TSubclassOf<UCommonButtonStyle> TouchStyle { get; }

    /// <inheritdoc />
    protected override void Construct_Implementation()
    {
        var inputSubsystem = GetLocalPlayerSubsystem<UCommonInputSubsystem>(OwningPlayerController);
        inputSubsystem.OnInputMethodChanged += HandleInputMethodChanged;
        HandleInputMethodChanged(inputSubsystem.CurrentInputType);
    }

    [UFunction]
    private void HandleInputMethodChanged(ECommonInputType newInputMethod)
    {
        TSubclassOf<UCommonButtonStyle> newStyle = default;
        Style = newInputMethod switch
        {
            ECommonInputType.MouseAndKeyboard => KeyboardStyle,
            ECommonInputType.Gamepad => GamepadStyle,
            ECommonInputType.Touch => TouchStyle,
            _ => default,
        };

        if (newStyle.Valid)
        {
            Style = newStyle;
        }
    }
}
