using InteractiveUI.Core.Selection;
using InteractiveUI.Core.Utilities;
using Pokemon.Core.Entities;
using Pokemon.UI.Components.Party;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.InteractiveUI;
using UnrealSharp.UMG;

namespace Pokemon.UI.Components.Common;

[UClass(ClassFlags.Abstract)]
public class UPokemonSelectionWidgetBase : UOwningSelectionWidget
{
    [UProperty(PropertyFlags.EditAnywhere, Category = "Display")]
    private TSubclassOf<UPokemonButtonBase> PanelClass { get; }

    [UProperty(PropertyFlags.EditAnywhere, Category = "Display")]
    private TSubclassOf<UWidget> BlankPanelClass { get; }

    [UProperty]
    private TArray<UWidget> BlankPanels { get; }

    public event Action<UPokemon, int>? OnPokemonHovered;

    public event Action<UPokemon, int>? OnPokemonSelected;

    protected virtual int? NumPanelsToAdd => null;

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

    public void Refresh()
    {
        foreach (var (_, panel) in Buttons.GetButtons<UPokemonPanel>())
        {
            panel.Refresh();
        }
    }

    [UFunction(
        FunctionFlags.BlueprintCallable | FunctionFlags.BlueprintEvent,
        Category = "Display"
    )]
    public virtual void ToggleCommandVisibility(bool visible) { }

    [UFunction(FunctionFlags.BlueprintEvent, Category = "Switching")]
    protected virtual void PerformSwap(UPokemonPanel panel1, UPokemonPanel panel2)
    {
        SwitchPokemon(panel1, panel2);
    }

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
