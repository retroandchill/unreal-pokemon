using InteractiveUI.Core.Selection;
using UnrealSharp.Attributes;
using UnrealSharp.Attributes.MetaTags;
using UnrealSharp.CommonUI;
using UnrealSharp.InteractiveUI;
using UnrealSharp.UMG;

namespace Pokemon.UI.Screens;

/// <summary>
/// Represents the Pause Menu screen in the application.
/// This class inherits from UCommonActivatableWidget and provides functionality for displaying
/// and interacting with the Pause Menu in the UI framework.
/// The Pause Menu includes options for navigation, and it integrates back action handling
/// to manage user interactions effectively.
/// </summary>
[UClass(ClassFlags.Abstract)]
public partial class UPauseMenuScreen : UCommonActivatableWidget
{
    [UProperty]
    [BindWidget]
    private partial USelectionWidget PauseMenuOptions { get; }

    /// <inheritdoc />
    protected override void Construct_Implementation()
    {
        PauseMenuOptions.OnBackAction += this.PopContentFromLayer;
    }

    /// <inheritdoc />
    protected override UWidget BP_GetDesiredFocusTarget_Implementation()
    {
        return PauseMenuOptions;
    }
}
