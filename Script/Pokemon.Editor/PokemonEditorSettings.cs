using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.DeveloperSettings;

namespace Pokemon.Editor;

[UClass(ClassFlags.DefaultConfig, DisplayName = "Pokémon Editor", ConfigCategory = "Game")]
public class UPokemonEditorSettings : UDeveloperSettings
{
    [UProperty(
        PropertyFlags.EditDefaultsOnly | PropertyFlags.BlueprintReadOnly | PropertyFlags.Config,
        Category = "Serializers"
    )]
    public FName NewGameplayTagsPath { get; }
}