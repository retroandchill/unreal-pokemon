using Pokemon.Core.Entities;
using Pokemon.UI.Components.Party;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.InteractiveUI;
using UnrealSharp.UMG;

namespace Pokemon.UI.Components.Common;

[UMultiDelegate]
public delegate void OnPokemonSelected(UPokemon pokemon);

[UClass(ClassFlags.Abstract)]
public class UPokemonSelectionWidgetBase : USelectableWidget, IPokemonPanelOwner
{
    [UProperty(PropertyFlags.EditAnywhere, Category = "Display")]
    private TSubclassOf<UPokemonPanel> PanelClass { get; }

    [UProperty(PropertyFlags.EditAnywhere, Category = "Display")]
    private TSubclassOf<UWidget> BlankPanelClass { get; }

    [UProperty]
    private TArray<UWidget> BlankPanels { get; }

    public int? SwitchingIndex { get; private set; }

    [UProperty(PropertyFlags.BlueprintAssignable, Category = "Events")]
    public TMulticastDelegate<OnPokemonSelected> OnPokemonSelected { get; set; }

    protected virtual int? NumPanelsToAdd => null;

    public UPokemonPanel? FindPanelForPokemon(UPokemon pokemon)
    {
        return GetSelectableOptions<UPokemonPanel>()
            .FirstOrDefault(x => ReferenceEquals(x.Pokemon, pokemon));
    }

    public void SetPokemonToDisplay(IReadOnlyList<UPokemon> pokemonList)
    {
        ClearSelectableOptions();
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
                newWidget.Owner = this;
                newWidget.ButtonIndex = i;
                newWidget.Pokemon = pokemonList[i];
                SlotOption(newWidget);
            }
            else
            {
                var newWidget = WidgetTree.ConstructWidget(BlankPanelClass);
                PlaceOptionIntoWidget(newWidget, i);
                BlankPanels.Add(newWidget);
            }
        }
    }

    public void Refresh()
    {
        foreach (var panel in GetSelectableOptions<UPokemonPanel>())
        {
            panel.Refresh();
        }
    }

    [UFunction(FunctionFlags.BlueprintCallable, Category = "Switching")]
    public void BeginSwitch(int startIndex)
    {
        if (SwitchingIndex.HasValue)
        {
            throw new InvalidOperationException("Already switching");
        }

        SwitchingIndex = startIndex;
        GetSelectableOption<UPokemonPanel>(startIndex)?.Refresh();
    }

    [UFunction(FunctionFlags.BlueprintCallable, Category = "Switching")]
    public void CompleteSwitch()
    {
        if (!SwitchingIndex.HasValue)
        {
            throw new InvalidOperationException("Not switching");
        }

        var panel1 = GetSelectableOption<UPokemonPanel>(SwitchingIndex.Value)!;
        var panel2 = GetSelectableOption<UPokemonPanel>(Index)!;
        SwitchingIndex = null;
        PerformSwap(panel1, panel2);
    }

    public void CancelSwitch()
    {
        if (!SwitchingIndex.HasValue)
        {
            throw new InvalidOperationException("Not switching");
        }

        var panel1 = GetSelectableOption<UPokemonPanel>(SwitchingIndex.Value)!;
        var panel2 = GetSelectableOption<UPokemonPanel>(Index)!;
        SwitchingIndex = null;
        panel1.Refresh();
        panel2.Refresh();
    }

    [UFunction(
        FunctionFlags.BlueprintCallable | FunctionFlags.BlueprintEvent,
        Category = "Display"
    )]
    public virtual void ToggleCommandVisibility(bool visible) { }

    protected override void OnSelectionChange(int oldIndex, int newIndex)
    {
        Refresh();
        if (newIndex != -1)
        {
            OnPokemonSelected.Invoke(GetSelectableOption<UPokemonPanel>(newIndex)!.Pokemon!);
        }
    }

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

        OnPokemonSelected.Invoke(GetSelectableOption<UPokemonPanel>(Index)!.Pokemon!);
    }
}
