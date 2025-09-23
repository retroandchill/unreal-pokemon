using System.Collections.Immutable;
using Pokemon.Core.Components.Trainer;
using Pokemon.Core.Entities;
using Pokemon.Core.Model.Trainer;
using RPG.SourceGenerator.Attributes;
using UnrealSharp.Attributes;
using UnrealSharp.RPGCore;
using UnrealSharp.RPGSaving;

namespace Pokemon.Core.Savers.Trainer;

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
                            () =>
                                throw new InvalidOperationException(
                                    $"Failed to save party member at index {i}"
                                )
                        )
                )
                .ToImmutableList(),
        };
    }

    private void LoadComponent(FPartyComponentInfo info, UPartyComponent component)
    {
        if (component.OwningEntity is not UTrainer trainer)
        {
            throw new InvalidOperationException(
                "Trying to load a party component on a non trainer entity"
            );
        }

        var subsystem = GetGameInstanceSubsystem<URPGSavingSubsystem>();

        foreach (var member in info.Party)
        {
            var pokemon = URPGEntity.CreateUninitialized(
                trainer,
                member.EntityClass.Cast<UPokemon>()
            );
            subsystem.LoadEntity(pokemon, member);
            component.AddPokemon(pokemon);
        }
    }
}
