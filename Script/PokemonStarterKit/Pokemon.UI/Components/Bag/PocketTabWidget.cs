using System.Diagnostics.CodeAnalysis;
using Pokemon.Core;
using Pokemon.Core.Entities;
using Pokemon.Data;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.Attributes.MetaTags;
using UnrealSharp.CommonUI;
using UnrealSharp.CoreUObject;
using UnrealSharp.EnhancedInput;
using UnrealSharp.GameplayTags;
using UnrealSharp.InteractiveUI;
using UnrealSharp.PokemonUI;
using UnrealSharp.UnrealSharpCommonUI;

namespace Pokemon.UI.Components.Bag;

[UClass]
public class UPocketTabWidget : UCommonUserWidget
{
    private FGameplayTag CurrentPocket
    {
        get;
        set
        {
            field = value;
            ItemSelectionWindow.CurrentPocket = field;
        }
    }

    [UProperty]
    [field: AllowNull, MaybeNull]
    public UItemSelectionWindow ItemSelectionWindow
    {
        get;
        set
        {
            field = value;
            field?.CurrentPocket = CurrentPocket;
        }
    }

    [UProperty]
    private TArray<UPocketButton> PocketButtons { get; set; }

    [UProperty]
    private UInteractiveButtonGroup PocketButtonGroup { get; set; }

    [UProperty(PropertyFlags.EditAnywhere, Category = "Buttons")]
    private TSubclassOf<UPocketButton> ButtonClass { get; }

    [UProperty(PropertyFlags.EditAnywhere | PropertyFlags.EditFixedSize, Category = "Buttons")]
    [Categories(IdentifierConstants.PocketTag)]
    private TMap<FGameplayTag, TSubclassOf<UCommonButtonStyle>> PocketButtonStyles { get; }

    [UProperty(PropertyFlags.EditAnywhere, Category = "Input")]
    private UInputAction PocketLeftAction { get; set; }

    [UProperty(PropertyFlags.EditAnywhere, Category = "Input")]
    private UInputAction PocketRightAction { get; set; }

    [UProperty]
    [BindWidget]
    private UCommonActionWidget PocketLeftActionWidget { get; }

    [UProperty]
    [BindWidget]
    private UCommonActionWidget PocketRightActionWidget { get; }

    public UPocketTabWidget()
    {
        var settings = GetDefault<UPokemonCoreSettings>();
        foreach (var (tag, _, _) in settings.Pockets)
        {
            PocketButtonStyles!.Add(tag, default);
        }
    }

    public override void PreConstruct(bool isDesignTime)
    {
        if (isDesignTime)
        {
            if (!ButtonClass.Valid)
                return;

            // ReSharper disable once ConditionalAccessQualifierIsNonNullableAccordingToAPIContract
            PocketButtonGroup?.RemoveAll();
        }

        PocketButtonGroup = NewObject<UInteractiveButtonGroup>(this);
        PocketButtonGroup.OnButtonAdded += [UFunction]
        (i, b) =>
        {
            var button = (UPocketButton)b;
            PocketButtons.Add(button);
            SlotButton(button);
        };
        PocketButtonGroup.OnButtonRemoved += [UFunction]
        (b) =>
        {
            var button = (UPocketButton)b;
            b.RemoveFromParent();
            PocketButtons.Remove(button);
        };
        PocketButtonGroup.OnSelectedButtonBaseChanged += [UFunction]
        (b, i) =>
        {
            var button = (UPocketButton)b;
            ItemSelectionWindow.CurrentPocket = button.Pocket;
        };
        PocketButtonGroup.OnButtonBaseClicked += PocketClicked;

        foreach (var (tag, _, _) in GetDefault<UPokemonCoreSettings>().Pockets)
        {
            var pocketButton = WidgetTree.ConstructWidget(ButtonClass);
            pocketButton.Pocket = tag;
            pocketButton.Style = PocketButtonStyles[tag];
            PocketButtonGroup.AddWidget(pocketButton);
        }

        PocketLeftActionWidget.EnhancedInputAction = PocketLeftAction;
        PocketRightActionWidget.EnhancedInputAction = PocketRightAction;
    }

    public override void Construct()
    {
        RegisterUIActionBinding(new FCSBindUIActionArgs(PocketLeftAction, false, PocketLeft));
        RegisterUIActionBinding(new FCSBindUIActionArgs(PocketRightAction, false, PocketLeft));

        var subsystem = GetGameInstanceSubsystem<UPokemonSubsystem>();
        var lastPocket = subsystem.Bag.LastViewedPocket;
        CurrentPocket = lastPocket.IsValid ? lastPocket : PocketButtons[0].Pocket;
    }

    [UFunction(FunctionFlags.BlueprintEvent, Category = "Buttons")]
    protected virtual void SlotButton(UPocketButton button)
    {
        // No native implementation
    }

    [UFunction]
    private void PocketLeft()
    {
        PocketButtonGroup.SelectPreviousButton();
    }

    [UFunction]
    private void PocketRight()
    {
        PocketButtonGroup.SelectNextButton();
    }

    [UFunction]
    private void PocketClicked(UCommonButtonBase button, int index)
    {
        var pocketButton = (UPocketButton)button;
        pocketButton.SetIsSelected(true);
        ItemSelectionWindow.CurrentPocket = pocketButton.Pocket;
    }
}
