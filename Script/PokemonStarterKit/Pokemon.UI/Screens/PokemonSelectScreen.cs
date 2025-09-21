using InteractiveUI.Core;
using InteractiveUI.Core.Selection;
using InteractiveUI.Core.Utilities;
using Pokemon.Core;
using Pokemon.Core.Entities;
using Pokemon.UI.Components.Common;
using Pokemon.UI.Components.Party;
using UnrealSharp.Attributes;
using UnrealSharp.Attributes.MetaTags;
using UnrealSharp.CommonUI;
using UnrealSharp.CoreUObject;
using UnrealSharp.Engine;
using UnrealSharp.InteractiveUI;
using UnrealSharp.UMG;
using IRefreshable = InteractiveUI.Core.IRefreshable;

namespace Pokemon.UI.Screens;

[UClass(ClassFlags.Abstract)]
public class UPokemonSelectScreen : UCommonActivatableWidget
{
    [UProperty(PropertyFlags.BlueprintReadOnly, Category = "Widgets")]
    [BindWidget]
    protected UPokemonSelectionPane SelectionPane { get; }
    
    [UProperty(PropertyFlags.BlueprintReadOnly, Category = "Widgets")]
    [BindWidgetOptional]
    protected UPokemonDisplayBase? PokemonDisplay { get; }
    
    [UProperty(PropertyFlags.BlueprintReadOnly, Category = "Widgets")]
    [BindWidget]
    public USelectionWidget CommandWidget { get; }
    
    [UProperty(PropertyFlags.BlueprintReadOnly, Category = "Widgets")]
    [BindWidgetOptional]
    public UPanelWidget? CommandPanel { get; }
    
    [UProperty(PropertyFlags.BlueprintReadOnly, Category = "State")]
    protected UPokemon CurrentPokemon
    {
        get;
        private set;
    }

    private Action<UPokemon?>? _onPokemonSelected;

    [UFunction(FunctionFlags.BlueprintCallable, Category = "Screens")]
    public static async Task<UPokemonSelectScreen> PushPokemonSelectScreenToStackAsync(
        APlayerController playerController,
        CancellationToken cancellationToken = default
    )
    {
        var screenClass = GetDefault<UPokemonUISettings>().PokemonSelectScreenClass;
        return await playerController.PushContentToLayerAsync(
            GameplayTags.UI_Layer_GameMenu,
            screenClass,
            cancellationToken
        );
    }

    public override void Construct()
    {
        CurrentPokemon = PokemonStatics.Player.FirstPartyPokemon;
        SelectionPane.OnPokemonHovered += OnPokemonHovered;
        SelectionPane.OnPokemonSelected += OnPokemonSelected;
        SelectionPane.OnBackAction += OnPokemonCancel;
    }

    [UFunction(FunctionFlags.BlueprintCallable, Category = "Switching")]
    public void BeginSwitch(int index)
    {
        SelectionPane.ToggleCommandVisibility(true);

        SelectionPane.DesiredFocusIndex = index;
        SelectionPane.BeginSwitch(index);
        SelectionPane.ActivateWidget();
    }

    [UFunction(FunctionFlags.BlueprintCallable, Category = "Switching")]
    public void Refresh()
    {
        SelectionPane.Refresh();
    }

    protected override UWidget? BP_GetDesiredFocusTarget()
    {
        return FocusUtilities.GetFirstActivatedWidget(CommandWidget, SelectionPane);
    }

    private void OnPokemonHovered(UPokemon pokemon, int index)
    {
        CurrentPokemon = pokemon;
        PokemonDisplay?.Pokemon = pokemon;
    }

    private void OnPokemonSelected(UPokemon pokemon, int index)
    {
        OnPokemonHovered(pokemon, index);
        var trainer = GetGameInstanceSubsystem<UPokemonSubsystem>().Player;
        if (_onPokemonSelected is not null)
        {
            _onPokemonSelected(pokemon);
            return;
        }

        if (SelectionPane.SwitchingIndex.HasValue)
        {
            if (SelectionPane.SwitchingIndex.Value != index)
            {
                trainer.SwapPositionsInParty(SelectionPane.SwitchingIndex.Value, index);
            }
            SelectionPane.CompleteSwitch();
        }
        else
        {
            DisplayPokemonCommands(pokemon, index);
        }
    }

    private void OnPokemonCancel()
    {
        if (_onPokemonSelected is not null)
        {
            _onPokemonSelected(null);
            return;
        }

        if (SelectionPane.SwitchingIndex.HasValue)
        {
            SelectionPane.CancelSwitch();
        }
        else
        {
            this.PopContentFromLayer();
        }
    }

    private void DisplayPokemonCommands(UPokemon pokemon, int index)
    {
        SelectionPane.DeactivateWidget();

        if (CommandWidget is IRefreshable refreshable)
        {
            refreshable.Refresh();
        }
        
        SetCommandPanelPosition(index);
        CommandWidget.DesiredFocusIndex = 0;
        CommandWidget.ActivateWidget();
    }

    private void SetCommandPanelPosition(int index)
    {
        if (CommandPanel?.Slot is not UCanvasPanelSlot canvasSlot) return;

        var button = SelectionPane.GetRequiredButton(index);
        var buttonGeometry = button.CachedGeometry;
        var selectionPaneGeometry = SelectionPane.CachedGeometry;
        
        var effectiveIndex = index / (SelectionPane.ButtonCount / 2);

        var buttonLocalSize = buttonGeometry.LocalSize;
        canvasSlot.Position = buttonGeometry.LocalTopLeft + new FVector2D(buttonLocalSize.X, effectiveIndex == 0 ? 0.0f : buttonLocalSize.Y) + selectionPaneGeometry.LocalTopLeft;
        canvasSlot.Alignment = new FVector2D(0.0f, effectiveIndex == 0 ? 0.0f : 1.0f);
    }

    public Task<UPokemon?> SelectPokemonAsync(CancellationToken cancellationToken = default)
    {
        if (_onPokemonSelected is not null)
        {
            throw new InvalidOperationException(
                "Cannot call SelectPokemonAsync when a callback is already registered"
            );
        }

        var tcs = new TaskCompletionSource<UPokemon?>(cancellationToken);

        _onPokemonSelected = handle =>
        {
            tcs.SetResult(handle);
            _onPokemonSelected = null;
        };

        return tcs.Task;
    }
}
