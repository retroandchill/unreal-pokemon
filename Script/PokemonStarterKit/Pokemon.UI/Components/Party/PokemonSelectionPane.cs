using Pokemon.Core;
using Pokemon.Core.Entities;
using Pokemon.UI.Components.Common;
using UnrealSharp.Attributes;
using UnrealSharp.InteractiveUI;

namespace Pokemon.UI.Components.Party;

[UClass(ClassFlags.Abstract)]
public class UPokemonSelectionPane : UPokemonSelectionWidgetBase
{
    protected override int? NumPanelsToAdd => GetDefault<UPokemonCoreSettings>().MaxPartySize;
    
    public override void Construct()
    {
        SetPokemonToDisplay(GetGameInstanceSubsystem<UPokemonSubsystem>().Player.PartyPokemon);
    }
}