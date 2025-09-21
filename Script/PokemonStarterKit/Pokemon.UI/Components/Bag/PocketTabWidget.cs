using System.Diagnostics.CodeAnalysis;
using InteractiveUI.Core.Selection;
using InteractiveUI.Core.Utilities;
using Pokemon.Core;
using Pokemon.Core.Entities;
using Pokemon.Data;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.Attributes.MetaTags;
using UnrealSharp.CommonUI;
using UnrealSharp.EnhancedInput;
using UnrealSharp.GameplayTags;
using UnrealSharp.InteractiveUI;
using UnrealSharp.UnrealSharpCommonUI;

namespace Pokemon.UI.Components.Bag;

[UClass]
public class UPocketTabWidget : UCommonActivatableWidget
{
    public FGameplayTag CurrentPocket
    {
        get;
        private set
        {
            if (field == value)
                return;
            field = value;

            Bag.LastViewedPocket = value;
            OnPocketChanged?.Invoke(value);
        }
    }

    [UProperty(PropertyFlags.Transient)]
    private UPokemonBag Bag { get; set; }

    public event Action<FGameplayTag>? OnPocketChanged;

    [UProperty(PropertyFlags.Transient)]
    private USelectableButtonGroup PocketButtonGroup { get; set; }

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

        PocketButtonGroup = NewObject<USelectableButtonGroup>(this);
        PocketButtonGroup.OnButtonAdded += [UFunction]
        (_, b) =>
        {
            SlotButton(b);
        };
        PocketButtonGroup.OnButtonRemoved += [UFunction]
        (b) =>
        {
            b.RemoveFromParent();
        };
        PocketButtonGroup.OnSelectedButtonBaseChanged += [UFunction]
        (b, i) =>
        {
            var button = (UPocketButton)b;
            CurrentPocket = button.Pocket;
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
        RegisterUIActionBinding(new FCSBindUIActionArgs(PocketRightAction, false, PocketRight));

        var subsystem = GetGameInstanceSubsystem<UPokemonSubsystem>();
        Bag = subsystem.Bag;
        var lastPocket = Bag.LastViewedPocket;
        if (lastPocket.IsValid)
        {
            CurrentPocket = lastPocket;
            var matchingButton = PocketButtonGroup
                .GetButtons<UPocketButton>()
                .Where(x => x.Button.Pocket == lastPocket)
                .Select(x => x.Button)
                .First();
            matchingButton.SetIsSelected(true);
        }
        else
        {
            var firstButton = PocketButtonGroup.GetRequiredButton<UPocketButton>(0);
            CurrentPocket = firstButton.Pocket;
        }
    }

    [UFunction(FunctionFlags.BlueprintEvent, Category = "Buttons")]
    protected virtual void SlotButton(UCommonButtonBase button)
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
        CurrentPocket = pocketButton.Pocket;
    }
}
