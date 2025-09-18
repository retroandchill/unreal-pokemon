using Pokemon.Core;
using Pokemon.Core.Entities;
using Pokemon.UI.Components.Party;
using UnrealSharp.Attributes;
using UnrealSharp.Attributes.MetaTags;
using UnrealSharp.CommonUI;
using UnrealSharp.InteractiveUI;

namespace Pokemon.UI.Screens;

public readonly record struct SelectedPokemonHandle(UPokemon Pokemon, UTrainer Trainer, int Index);

[UClass(ClassFlags.Abstract)]
public class UPokemonSelectScreen : UCommonActivatableWidget
{
    [UProperty(PropertyFlags.BlueprintReadOnly, Category = "Widgets")]
    [BindWidget]
    protected UPokemonSelectionPane SelectionPane { get; }

    private Action<SelectedPokemonHandle>? _onPokemonSelected;

    public override void Construct()
    {
        SelectionPane.Index = 0;
        SelectionPane.ActivateWidget();
    }

    [UFunction(FunctionFlags.BlueprintCallable, Category = "Switching")]
    public void BeginSwitch(int index)
    {
        SelectionPane.ToggleCommandVisibility(true);

        SelectionPane.Index = index;
        SelectionPane.BeginSwitch(index);
        SelectionPane.ActivateWidget();
    }

    [UFunction(FunctionFlags.BlueprintCallable, Category = "Switching")]
    public void Refresh()
    {
        SelectionPane.Refresh();
    }

    [UFunction(FunctionFlags.BlueprintCallable, Category = "Selection")]
    protected void OnPokemonSelected(int index)
    {
        var trainer = GetGameInstanceSubsystem<UPokemonSubsystem>().Player;
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
            DisplayPokemonCommands(trainer, index);
        }
    }

    [UFunction(FunctionFlags.BlueprintCallable, Category = "Selection")]
    protected void OnPokemonCancel()
    {
        if (SelectionPane.SwitchingIndex.HasValue)
        {
            SelectionPane.CancelSwitch();
        }
        else
        {
            this.PopContentFromLayer();
        }
    }

    [UFunction(FunctionFlags.BlueprintEvent, Category = "Selection")]
    protected virtual void DisplayPokemonCommands(UTrainer trainer, int index) { }

    public Task<SelectedPokemonHandle> SelectPokemonAsync(
        CancellationToken cancellationToken = default
    )
    {
        if (_onPokemonSelected is not null)
        {
            throw new InvalidOperationException(
                "Cannot call SelectPokemonAsync when a callback is already registered"
            );
        }

        var tcs = new TaskCompletionSource<SelectedPokemonHandle>(cancellationToken);

        _onPokemonSelected = handle =>
        {
            tcs.SetResult(handle);
            _onPokemonSelected = null;
        };

        return tcs.Task;
    }
}
