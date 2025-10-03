using System.Diagnostics.CodeAnalysis;
using InteractiveUI.Core.Utilities;
using Pokemon.Core;
using Pokemon.Core.Entities;
using Pokemon.UI.Components.Common;
using UnrealSharp.Attributes;
using UnrealSharp.InteractiveUI;

namespace Pokemon.UI.Components.Party;

/// <summary>
/// Manages the UI pane for selecting Pokémon in the player's party, allowing switch, cancel, and complete switch operations.
/// Extends functionality of <see cref="UPokemonSelectionWidgetBase"/>.
/// Used primarily for handling Pokémon selection and management within the party.
/// </summary>
[UClass(ClassFlags.Abstract)]
public class UPokemonSelectionPane : UPokemonSelectionWidgetBase
{
    /// <summary>
    /// Represents the index of the currently selected Pokémon in the switching process.
    /// </summary>
    /// <remarks>
    /// This property holds a nullable integer value that corresponds to the index of the Pokémon
    /// being switched in the party selection pane. It is set when a switch begins and cleared
    /// upon completing or canceling the switch process.
    /// </remarks>
    public int? SwitchingIndex { get; private set; }

    /// <summary>
    /// Indicates whether a Pokémon switch operation is currently in progress.
    /// </summary>
    /// <remarks>
    /// This property returns true if the switching process has been initiated and an index is set in the <see cref="SwitchingIndex"/> property.
    /// It is primarily used to manage the state of the Pokémon selection pane during the switch operation.
    /// </remarks>
    [MemberNotNullWhen(true, nameof(SwitchingIndex))]
    public bool IsSwitching => SwitchingIndex.HasValue;

    /// <inheritdoc />
    protected override int? NumPanelsToAdd => GetDefault<UPokemonCoreSettings>().MaxPartySize;

    /// <inheritdoc />
    public override void Construct()
    {
        base.Construct();
        SetPokemonToDisplay(GetGameInstanceSubsystem<UPokemonSubsystem>().Player.PartyPokemon);
    }

    /// <summary>
    /// Initiates the Pokémon switching process by setting the starting index
    /// for the switch and refreshing the relevant Pokémon panel.
    /// </summary>
    /// <param name="startIndex">The index of the Pokémon panel to begin the switch operation.</param>
    /// <exception cref="InvalidOperationException">Thrown if a switch operation is already in progress.</exception>
    [UFunction(FunctionFlags.BlueprintCallable, Category = "Switching")]
    public void BeginSwitch(int startIndex)
    {
        if (SwitchingIndex.HasValue)
        {
            throw new InvalidOperationException("Already switching");
        }

        var switchingButton = Buttons.GetButton<UPokemonPanel>(startIndex);
        SwitchingIndex = startIndex;
        switchingButton?.Refresh();
    }

    /// <summary>
    /// Completes the process of switching Pokemon in the selection pane by swapping the positions
    /// of two Pokemon panels. The method retrieves the current panel being switched and the target
    /// panel based on the desired focus index, performs the swap, and then resets the switching state.
    /// </summary>
    /// <exception cref="InvalidOperationException">Thrown if no switching is in progress.</exception>
    [UFunction(FunctionFlags.BlueprintCallable, Category = "Switching")]
    public void CompleteSwitch()
    {
        if (!SwitchingIndex.HasValue)
        {
            throw new InvalidOperationException("Not switching");
        }

        var panel1 = Buttons.GetRequiredButton<UPokemonPanel>(SwitchingIndex.Value);
        var panel2 = Buttons.GetRequiredButton<UPokemonPanel>(DesiredFocusIndex);
        SwitchingIndex = null;
        PerformSwap(panel1, panel2);
    }

    /// <summary>
    /// Cancels the in-progress switch operation within the Pokémon selection pane.
    /// </summary>
    /// <exception cref="InvalidOperationException">
    /// Thrown when there is no active switch operation to cancel.
    /// </exception>
    public void CancelSwitch()
    {
        if (!SwitchingIndex.HasValue)
        {
            throw new InvalidOperationException("Not switching");
        }

        var panel1 = Buttons.GetRequiredButton<UPokemonPanel>(SwitchingIndex.Value);
        var panel2 = Buttons.GetRequiredButton<UPokemonPanel>(DesiredFocusIndex);
        SwitchingIndex = null;
        panel1.Refresh();
        panel2.Refresh();
    }
}
