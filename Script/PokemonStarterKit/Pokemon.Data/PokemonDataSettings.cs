using UnrealSharp.Attributes;
using UnrealSharp.Attributes.MetaTags;
using UnrealSharp.DeveloperSettings;

namespace Pokemon.Data;

[UClass(ClassFlags.DefaultConfig, DisplayName = "Pokémon Data", ConfigCategory = "Game")]
public class UPokemonDataSettings : UDeveloperSettings
{
    [UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditDefaultsOnly | PropertyFlags.Config,
        Category = "Gameplay"
    )]
    [ClampMin("1")]
    public int MaxLevel { get; init; } = 100;

    [UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditDefaultsOnly | PropertyFlags.Config,
        Category = "Gameplay"
    )]
    public bool MoveCategoryPerMove { get; init; } = false;
}
