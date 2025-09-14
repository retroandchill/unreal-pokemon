using JetBrains.Annotations;
using Pokemon.Core.Entities;
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
        Category = "Pokémon"
    )]
    [UsedImplicitly]
    public int MaxMoves { get; } = 4;

    [UProperty(
        PropertyFlags.EditDefaultsOnly | PropertyFlags.BlueprintReadOnly | PropertyFlags.Config,
        Category = "Pokémon"
    )]
    [UsedImplicitly]
    public int ShinyPokemonChance { get; } = 16;

    [UProperty(
        PropertyFlags.EditDefaultsOnly | PropertyFlags.BlueprintReadOnly | PropertyFlags.Config,
        Category = "DefaultClasses"
    )]
    public TSoftClassPtr<UPokemon> PokemonClass { get; }

    [UProperty(
        PropertyFlags.EditDefaultsOnly | PropertyFlags.BlueprintReadOnly | PropertyFlags.Config,
        Category = "DefaultClasses"
    )]
    public TSoftClassPtr<UTrainer> TrainerClass { get; }
}
