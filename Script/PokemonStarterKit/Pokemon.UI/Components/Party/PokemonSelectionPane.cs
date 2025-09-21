using System.Diagnostics.CodeAnalysis;
using InteractiveUI.Core.Utilities;
using Pokemon.Core;
using Pokemon.Core.Entities;
using Pokemon.UI.Components.Common;
using UnrealSharp.Attributes;
using UnrealSharp.InteractiveUI;

namespace Pokemon.UI.Components.Party;

[UClass(ClassFlags.Abstract)]
public class UPokemonSelectionPane : UPokemonSelectionWidgetBase
{
    public int? SwitchingIndex { get; private set; }

    [MemberNotNullWhen(true, nameof(SwitchingIndex))]
    public bool IsSwitching => SwitchingIndex.HasValue;

    protected override int? NumPanelsToAdd => GetDefault<UPokemonCoreSettings>().MaxPartySize;

    public override void Construct()
    {
        base.Construct();
        SetPokemonToDisplay(GetGameInstanceSubsystem<UPokemonSubsystem>().Player.PartyPokemon);
    }

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
