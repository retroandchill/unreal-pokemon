using Pokemon.UI.Screens;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.DeveloperSettings;

namespace Pokemon.UI;

[UClass(ClassFlags.DefaultConfig, DisplayName = "Pokémon UI", ConfigCategory = "Game")]
public class UPokemonUISettings : UDeveloperSettings
{
    
    [UProperty(
        PropertyFlags.EditDefaultsOnly | PropertyFlags.BlueprintReadOnly | PropertyFlags.Config,
        Category = "Classes"
    )]
    public TSoftClassPtr<UPokemonSelectScreen> PokemonSelectScreenClass { get; }
    
    [UProperty(
        PropertyFlags.EditDefaultsOnly | PropertyFlags.BlueprintReadOnly | PropertyFlags.Config,
        Category = "Classes"
    )]
    public TSoftClassPtr<UBagScreen> BagScreenClass { get; }
}
