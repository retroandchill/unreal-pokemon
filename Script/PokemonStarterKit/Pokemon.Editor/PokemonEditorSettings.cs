using Pokemon.Data;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.Attributes.MetaTags;
using UnrealSharp.DeveloperSettings;
using UnrealSharp.GameplayTags;

namespace Pokemon.Editor;

[UClass(ClassFlags.DefaultConfig, DisplayName = "Pokémon Editor", ConfigCategory = "Game")]
public class UPokemonEditorSettings : UDeveloperSettings
{
    [UProperty(
        PropertyFlags.EditDefaultsOnly | PropertyFlags.BlueprintReadOnly | PropertyFlags.Config,
        Category = "Serializers"
    )]
    public FName NewGameplayTagsPath { get; }
    
    [UProperty(
        PropertyFlags.EditDefaultsOnly | PropertyFlags.BlueprintReadOnly | PropertyFlags.Config,
        Category = "Pbs"
    )]
    [ClampMin("1")]
    [UIMax("1")]
    [Categories(IdentifierConstants.PocketTag)]
    public IReadOnlyDictionary<int, FGameplayTag> PocketNumberToGameplayTag { get; }
}