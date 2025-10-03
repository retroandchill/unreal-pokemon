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

/// <summary>
/// Represents the UI for selecting a Pokémon within the Pokémon Select Screen.
/// Handles displaying available Pokémon, managing user interaction, and responding
/// to selection or hover events.
/// </summary>
[UClass(ClassFlags.Abstract)]
public class UPokemonSelectScreen : UCommonActivatableWidget
{
    /// <summary>
    /// Represents the selection pane used within the Pokémon Select Screen.
    /// This property manages interactions related to displaying a list of Pokémon,
    /// user navigation, and triggering selection/hovers on the listed Pokémon.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly, Category = "Widgets")]
    [BindWidget]
    protected UPokemonSelectionPane SelectionPane { get; }

    /// <summary>
    /// Represents the display widget for presenting details or visuals of a Pokémon
    /// within the Pokémon Select Screen. This property is responsible for showing
    /// individual Pokémon's appearance or attributes as they are selected or hovered.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly, Category = "Widgets")]
    [BindWidgetOptional]
    protected UPokemonDisplayBase? PokemonDisplay { get; }

    /// <summary>
    /// Represents the command widget used to display and manage user-interactable commands
    /// associated with Pokémon selections in the Pokémon Select Screen.
    /// Handles activation, focus management, and command-related interactions.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly, Category = "Widgets")]
    [BindWidget]
    public USelectionWidget CommandWidget { get; }

    /// <summary>
    /// Represents the command panel within the Pokémon Select Screen.
    /// This property is used to display and position user interaction elements
    /// related to command selection, such as navigation buttons or commands for Pokémon.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly, Category = "Widgets")]
    [BindWidgetOptional]
    public UPanelWidget? CommandPanel { get; }

    /// <summary>
    /// Represents the currently selected or hovered Pokémon in the Pokémon Select Screen.
    /// This property is used to keep track of the active Pokémon based on user interaction,
    /// such as selecting from the available choices or highlighting during navigation.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly, Category = "State")]
    protected UPokemon CurrentPokemon { get; private set; }

    private Action<UPokemon?>? _onPokemonSelected;

    /// <summary>
    /// Asynchronously pushes the <c>UPokemonSelectScreen</c> to the game menu layer stack for display.
    /// </summary>
    /// <param name="playerController">The player controller instance responsible for managing the UI stack.</param>
    /// <param name="cancellationToken">An optional CancellationToken to observe while waiting for the operation to complete.</param>
    /// <returns>A task representing the asynchronous operation that resolves to an instance of the <c>UPokemonSelectScreen</c> being displayed.</returns>
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

    /// <inheritdoc />
    public override void Construct()
    {
        CurrentPokemon = PokemonStatics.Player.FirstPartyPokemon;
        SelectionPane.OnPokemonHovered += OnPokemonHovered;
        SelectionPane.OnPokemonSelected += OnPokemonSelected;
        SelectionPane.OnBackAction += OnPokemonCancel;
    }

    /// <summary>
    /// Initiates a switch to the specified selection index in the <c>UPokemonSelectionPane</c>, updating the focus and activating the widget.
    /// </summary>
    /// <param name="index">The desired index to which the selection focus should switch.</param>
    [UFunction(FunctionFlags.BlueprintCallable, Category = "Switching")]
    public void BeginSwitch(int index)
    {
        SelectionPane.ToggleCommandVisibility(true);

        SelectionPane.DesiredFocusIndex = index;
        SelectionPane.BeginSwitch(index);
        SelectionPane.ActivateWidget();
    }

    /// <summary>
    /// Refreshes the Pokémon selection pane by updating its content and state.
    /// This method ensures that any changes in the data or UI state are reflected
    /// in the selection pane display.
    /// </summary>
    [UFunction(FunctionFlags.BlueprintCallable, Category = "Switching")]
    public void Refresh()
    {
        SelectionPane.Refresh();
    }

    /// <inheritdoc />
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
        if (CommandPanel?.Slot is not UCanvasPanelSlot canvasSlot)
            return;

        var button = SelectionPane.GetRequiredButton(index);
        var buttonGeometry = button.CachedGeometry;
        var selectionPaneGeometry = SelectionPane.CachedGeometry;

        var effectiveIndex = index / (SelectionPane.ButtonCount / 2);

        var buttonLocalSize = buttonGeometry.LocalSize;
        canvasSlot.Position =
            buttonGeometry.LocalTopLeft
            + new FVector2D(buttonLocalSize.X, effectiveIndex == 0 ? 0.0f : buttonLocalSize.Y)
            + selectionPaneGeometry.LocalTopLeft;
        canvasSlot.Alignment = new FVector2D(0.0f, effectiveIndex == 0 ? 0.0f : 1.0f);
    }

    /// <summary>
    /// Asynchronously allows the user to select a Pokemon from a list or party.
    /// </summary>
    /// <param name="cancellationToken">An optional CancellationToken to observe while waiting for the operation to complete.</param>
    /// <returns>A task representing the asynchronous operation that resolves to the selected <c>UPokemon</c>, or null if no Pokemon was selected.</returns>
    public Task<UPokemon?> SelectPokemonAsync(CancellationToken cancellationToken = default)
    {
        if (_onPokemonSelected is not null)
        {
            throw new InvalidOperationException("Cannot call SelectPokemonAsync when a callback is already registered");
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
