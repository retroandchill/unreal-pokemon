using JetBrains.Annotations;
using Pokemon.Core.Entities;
using Pokemon.Core.Entities.Components;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.DeveloperSettings;

namespace Pokemon.Core;

[UClass(ClassFlags.DefaultConfig, DisplayName = "Pokémon Core", ConfigCategory = "Game")]
public class UPokemonCoreSettings : UDeveloperSettings
{
    [UProperty(
        PropertyFlags.EditDefaultsOnly | PropertyFlags.BlueprintReadOnly | PropertyFlags.Config,
        Category = "Trainers"
    )]
    [UsedImplicitly]
    public int MaxPartySize { get; } = 6;

    [UProperty(
        PropertyFlags.EditDefaultsOnly | PropertyFlags.BlueprintReadOnly | PropertyFlags.Config,
        Category = "DefaultClasses"
    )]
    public TSubclassOf<UPokemon> PokemonClass { get; }
}
