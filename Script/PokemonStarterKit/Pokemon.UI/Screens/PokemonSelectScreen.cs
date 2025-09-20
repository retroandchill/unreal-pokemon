using InteractiveUI.Core;
using Pokemon.Core;
using Pokemon.Core.Entities;
using Pokemon.UI.Components.Party;
using UnrealSharp.Attributes;
using UnrealSharp.Attributes.MetaTags;
using UnrealSharp.CommonUI;
using UnrealSharp.Engine;
using UnrealSharp.InteractiveUI;

namespace Pokemon.UI.Screens;

[UClass(ClassFlags.Abstract)]
public class UPokemonSelectScreen : UCommonActivatableWidget
{
    [UProperty(PropertyFlags.BlueprintReadOnly, Category = "Widgets")]
    [BindWidget]
    protected UPokemonSelectionPane SelectionPane { get; }

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

    [UFunction(FunctionFlags.BlueprintCallable, Category = "Selection")]
    protected void OnPokemonSelected(int index)
    {
        var trainer = GetGameInstanceSubsystem<UPokemonSubsystem>().Player;
        if (_onPokemonSelected is not null)
        {
            _onPokemonSelected(trainer.PartyPokemon[index]);
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
            DisplayPokemonCommands(trainer, index);
        }
    }

    [UFunction(FunctionFlags.BlueprintCallable, Category = "Selection")]
    protected void OnPokemonCancel()
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

    [UFunction(FunctionFlags.BlueprintEvent, Category = "Selection")]
    protected virtual void DisplayPokemonCommands(UTrainer trainer, int index) { }

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
