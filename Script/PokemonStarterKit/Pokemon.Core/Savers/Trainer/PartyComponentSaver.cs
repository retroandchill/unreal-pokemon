using System.Collections.Immutable;
using Pokemon.Core.Components.Trainer;
using Pokemon.Core.Entities;
using Pokemon.Core.Model.Trainer;
using RPG.SourceGenerator.Attributes;
using UnrealSharp.Attributes;
using UnrealSharp.RPGCore;
using UnrealSharp.RPGSaving;

namespace Pokemon.Core.Savers.Trainer;

/// <summary>
/// Handles the saving logic for the <see cref="UPartyComponent"/> instances and facilitates persistence of
/// data encapsulated in the corresponding <see cref="FPartyComponentInfo"/> structure. This class is tailored
/// specifically for managing trainer's party-related components in the context of RPG saving mechanisms.
/// </summary>
/// <remarks>
/// The <see cref="UPartyComponentSaver"/> inherits functionality from the <see cref="UComponentSaver"/>
/// class to provide a framework for saving data associated with gameplay components.
/// </remarks>
[UClass]
[ComponentSaver<UPartyComponent, FPartyComponentInfo>(LoggerClass = typeof(LogPokemonCore))]
public partial class UPartyComponentSaver : UComponentSaver
{
    private FPartyComponentInfo SaveComponent(UPartyComponent component)
    {
        var subsystem = GetGameInstanceSubsystem<URPGSavingSubsystem>();
        return new FPartyComponentInfo
        {
            Party = component
                .PartyPokemon.Select(p => subsystem.SaveEntity(p))
                .Select(
                    (o, i) =>
                        o.Match(
                            p => p,
                            () => throw new InvalidOperationException($"Failed to save party member at index {i}")
                        )
                )
                .ToImmutableList(),
        };
    }

    private void LoadComponent(FPartyComponentInfo info, UPartyComponent component)
    {
        if (component.OwningEntity is not UTrainer trainer)
        {
            throw new InvalidOperationException("Trying to load a party component on a non trainer entity");
        }

        var subsystem = GetGameInstanceSubsystem<URPGSavingSubsystem>();

        foreach (var member in info.Party)
        {
            var pokemon = URPGEntity.CreateUninitialized(trainer, member.EntityClass.Cast<UPokemon>());
            subsystem.LoadEntity(pokemon, member);
            component.AddPokemon(pokemon);
        }
    }
}
