using InteractiveUI.Core.Selection;
using InteractiveUI.Core.Utilities;
using Pokemon.Core.Entities;
using Pokemon.UI.Components.Party;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.InteractiveUI;
using UnrealSharp.UMG;

namespace Pokemon.UI.Components.Common;

/// <summary>
/// Base class for a widget that manages the selection of Pokémon within the user interface.
/// </summary>
/// <remarks>
/// This class provides core functionality for widgets that allow users to hover over, select, and swap Pokémon.
/// It handles interactions with buttons, updating the display, and supports customization through inheritance. It is
/// designed to be extended by other widgets for specific use cases.
/// </remarks>
[UClass(ClassFlags.Abstract)]
public class UPokemonSelectionWidgetBase : UOwningSelectionWidget
{
    /// <summary>
    /// Gets the class type used to create panel widgets within the Pokémon selection widget.
    /// </summary>
    /// <remarks>
    /// This property specifies the type of widget to use for displaying individual Pokémon panels.
    /// It is intended to be assigned with a subclass of UPokemonButtonBase.
    /// The property enables customization and dynamic creation of Pokémon panels at runtime.
    /// </remarks>
    [UProperty(PropertyFlags.EditAnywhere, Category = "Display")]
    private TSubclassOf<UPokemonButtonBase> PanelClass { get; }

    /// <summary>
    /// Gets the class type used to create blank panel widgets within the Pokémon selection widget.
    /// </summary>
    /// <remarks>
    /// This property defines the type of widget to use for generating blank panels in the UI when there are no Pokémon to display.
    /// The specified class type must be a subclass of UWidget.
    /// It allows for flexible customization and dynamic instantiation of empty panel slots at runtime.
    /// </remarks>
    [UProperty(PropertyFlags.EditAnywhere, Category = "Display")]
    private TSubclassOf<UWidget> BlankPanelClass { get; }

    [UProperty]
    private TArray<UWidget> BlankPanels { get; }

    /// <summary>
    /// Event triggered when a Pokémon is hovered over in the selection widget.
    /// </summary>
    /// <remarks>
    /// This event is fired whenever a user hovers over a Pokémon in the selection interface. It provides
    /// a reference to the hovered Pokémon and its corresponding index within the current list.
    /// Subscribers to this event can use it to update UI elements or execute specific logic based on the hovered Pokémon.
    /// </remarks>
    public event Action<UPokemon, int>? OnPokemonHovered;

    /// <summary>
    /// Event triggered when a Pokémon is selected within the selection widget.
    /// </summary>
    /// <remarks>
    /// This event is invoked when a user selects a Pokémon from the available options in the widget. It provides the selected
    /// Pokémon object and its associated index within the selection list. Components or systems subscribing to this event can
    /// utilize the information to perform operations such as updating the UI, handling team composition, or processing gameplay
    /// logic related to the selected Pokémon.
    /// </remarks>
    public event Action<UPokemon, int>? OnPokemonSelected;

    /// <summary>
    /// Determines the number of additional panels to generate within the Pokémon selection widget.
    /// </summary>
    /// <remarks>
    /// This property specifies the upper limit of panels to display for selecting or managing Pokémon.
    /// When set to a value, it overrides the default generation count based on the Pokémon list size.
    /// It is typically influenced by configuration settings or gameplay constraints, such as the maximum party size.
    /// </remarks>
    protected virtual int? NumPanelsToAdd => null;

    /// <inheritdoc />
    public override void Construct()
    {
        base.Construct();
        Buttons.OnHoveredButtonBaseChanged += [UFunction]
        (button, i) =>
        {
            if (button is UPokemonButtonBase { Pokemon: not null } pokemonButton)
            {
                OnPokemonHovered?.Invoke(pokemonButton.Pokemon, i);
            }
        };
        Buttons.OnButtonBaseClicked += [UFunction]
        (button, i) =>
        {
            if (button is UPokemonButtonBase { Pokemon: not null } pokemonButton)
            {
                OnPokemonSelected?.Invoke(pokemonButton.Pokemon, i);
            }
        };
    }

    /// <summary>
    /// Configures and updates the display of Pokémon objects within the selection widget.
    /// Removes existing widgets, clears blank panels, and dynamically creates new widgets or blank panels
    /// based on the input Pokémon list and the specified constraints.
    /// </summary>
    /// <param name="pokemonList">A read-only list of Pokémon to be displayed. Determines the content of the dynamically created widgets.</param>
    public void SetPokemonToDisplay(IReadOnlyList<UPokemon> pokemonList)
    {
        Buttons.RemoveAll();
        foreach (var blankPanel in BlankPanels)
        {
            blankPanel.RemoveFromParent();
        }
        BlankPanels.Clear();

        var maxPartySize = NumPanelsToAdd ?? pokemonList.Count;
        for (var i = 0; i < maxPartySize; i++)
        {
            if (i < pokemonList.Count)
            {
                var name = $"selectionPanel{i}";
                var newWidget = WidgetTree.ConstructWidget(PanelClass, name);
                newWidget.Pokemon = pokemonList[i];
                Buttons.AddWidget(newWidget);
            }
            else
            {
                var newWidget = WidgetTree.ConstructWidget(BlankPanelClass);
                SlotButton(i, newWidget);
                BlankPanels.Add(newWidget);
            }
        }
    }

    /// <summary>
    /// Refreshes the state of all Pokémon panels managed by the widget.
    /// </summary>
    /// <remarks>
    /// This method iterates over the collection of buttons associated with Pokémon panels
    /// and invokes the `Refresh` method on each panel. Designed to update the panel display
    /// to reflect the current state or data.
    /// </remarks>
    public void Refresh()
    {
        foreach (var (_, panel) in Buttons.GetButtons<UPokemonPanel>())
        {
            panel.Refresh();
        }
    }

    /// <summary>
    /// Toggles the visibility of command UI elements.
    /// </summary>
    /// <param name="visible">A boolean value indicating whether the command elements should be visible (true) or hidden (false).</param>
    [UFunction(FunctionFlags.BlueprintCallable | FunctionFlags.BlueprintEvent, Category = "Display")]
    public virtual void ToggleCommandVisibility(bool visible) { }

    /// <summary>
    /// Performs a swap action between two Pokémon panels.
    /// </summary>
    /// <param name="panel1">The first Pokémon panel involved in the swap.</param>
    /// <param name="panel2">The second Pokémon panel involved in the swap.</param>
    [UFunction(FunctionFlags.BlueprintEvent, Category = "Switching")]
    protected virtual void PerformSwap(UPokemonPanel panel1, UPokemonPanel panel2)
    {
        SwitchPokemon(panel1, panel2);
    }

    /// <summary>
    /// Swaps the Pokémon between two panels and updates their display. Also triggers the OnPokemonHovered event with the currently focused Pokémon and its index.
    /// </summary>
    /// <param name="panel1">The first Pokémon panel involved in the swap.</param>
    /// <param name="panel2">The second Pokémon panel involved in the swap.</param>
    [UFunction(FunctionFlags.BlueprintCallable, Category = "Switching")]
    protected void SwitchPokemon(UPokemonPanel panel1, UPokemonPanel panel2)
    {
        panel1.SwapPokemon(panel2);
        panel1.Refresh();
        panel2.Refresh();

        OnPokemonHovered?.Invoke(
            Buttons.GetRequiredButton<UPokemonPanel>(DesiredFocusIndex).Pokemon!,
            DesiredFocusIndex
        );
    }
}
