using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.CommonInput;
using UnrealSharp.CommonUI;

namespace InteractiveUI.Core.Actions;

[UClass(ClassFlags.Abstract)]
[UMetaData("DisableNativeTick")]
public class UInteractiveActionButton : UCommonBoundActionButton
{
    [UProperty(PropertyFlags.EditAnywhere, Category = "Style")]
    private TSubclassOf<UCommonButtonStyle> KeyboardStyle { get; }
    
    [UProperty(PropertyFlags.EditAnywhere, Category = "Style")]
    private TSubclassOf<UCommonButtonStyle> GamepadStyle { get; }
    
    [UProperty(PropertyFlags.EditAnywhere, Category = "Style")]
    private TSubclassOf<UCommonButtonStyle> TouchStyle { get; }
    
    public override void Construct()
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
            _ => default
        };

        if (newStyle.Valid)
        {
            Style = newStyle;
        }
    }
}