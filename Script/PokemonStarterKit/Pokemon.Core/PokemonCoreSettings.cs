using JetBrains.Annotations;
using LanguageExt;
using Pokemon.Core.Entities;
using Pokemon.Data;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.Attributes.MetaTags;
using UnrealSharp.DeveloperSettings;
using UnrealSharp.GameplayTags;

namespace Pokemon.Core;

[UStruct]
public readonly partial record struct FPocketSetting(
    [field: UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere)]
    [field: Categories(IdentifierConstants.PocketTag)]
        FGameplayTag Tag,
    [field: UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere)] FText Name,
    [field: UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere)]
        Option<int> MaxSize
);

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
        Category = "Bag"
    )]
    [UsedImplicitly]
    public TArray<FPocketSetting> Pockets { get; }

    [UProperty(
        PropertyFlags.EditDefaultsOnly | PropertyFlags.BlueprintReadOnly | PropertyFlags.Config,
        Category = "Bag"
    )]
    [UsedImplicitly]
    public int MaxPerSlot { get; } = 999;

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

    [UProperty(
        PropertyFlags.EditDefaultsOnly | PropertyFlags.BlueprintReadOnly | PropertyFlags.Config,
        Category = "DefaultClasses"
    )]
    public TSoftClassPtr<UPokemonBag> BagClass { get; }
}
