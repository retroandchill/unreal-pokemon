using InteractiveUI.Core.Selection;
using UnrealSharp.Attributes;
using UnrealSharp.Attributes.MetaTags;
using UnrealSharp.CommonUI;
using UnrealSharp.InteractiveUI;
using UnrealSharp.UMG;

namespace Pokemon.UI.Screens;

[UClass(ClassFlags.Abstract)]
public class UPauseMenuScreen : UCommonActivatableWidget
{
    [UProperty]
    [BindWidget]
    private USelectionWidget PauseMenuOptions { get; }

    public override void Construct()
    {
        PauseMenuOptions.OnBackAction += this.PopContentFromLayer;
    }

    protected override UWidget BP_GetDesiredFocusTarget()
    {
        return PauseMenuOptions;
    }
}