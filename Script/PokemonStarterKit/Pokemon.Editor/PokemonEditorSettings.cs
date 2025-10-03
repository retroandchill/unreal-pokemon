using GameDataAccessTools.Core.Utilities;
using Pokemon.Data;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.Attributes.MetaTags;
using UnrealSharp.GameplayTags;
using UnrealSharp.UnrealSharpCore;

namespace Pokemon.Editor;

/// <summary>
/// Represents the editor settings for the Pokémon Editor. This class allows configuration of gameplay-related settings
/// in the editor, such as tag management and mappings between pocket numbers and gameplay tags.
/// </summary>
/// <remarks>
/// This class extends UCSDeveloperSettings to provide customized configurations for Pokémon-related editing tools.
/// It includes properties for managing tag paths and mappings, which interact with other systems for importing or
/// exporting gameplay-related data.
/// </remarks>
[UClass(ClassFlags.DefaultConfig, ConfigCategory = "Editor")]
public class UPokemonEditorSettings : UCSDeveloperSettings
{
    /// <summary>
    /// The file path used to store or reference newly created gameplay tags within the Pokémon Editor.
    /// </summary>
    /// <remarks>
    /// This property is utilized to specify a configuration path where new gameplay tags can be added or managed.
    /// It serves as a centralized location for defining tag data that can be accessed or modified through the editor's tools.
    /// Modifications to this path directly impact how new tags are imported and managed.
    /// </remarks>
    [UProperty(
        PropertyFlags.EditDefaultsOnly | PropertyFlags.BlueprintReadOnly | PropertyFlags.Config,
        Category = "Serializers"
    )]
    public FName NewGameplayTagsPath { get; }

    /// <summary>
    /// A mapping between pocket numbers and their corresponding gameplay tags used within the Pokémon Editor.
    /// </summary>
    /// <remarks>
    /// This property provides an association between integer-based pocket numbers and predefined gameplay tags.
    /// It is primarily utilized to configure and manage gameplay logic that depends on pockets, helping to
    /// identify and categorize gameplay elements based on the assigned tags. Modifications to this mapping
    /// influence how pockets are interpreted and interacted with in the editor and during gameplay.
    /// </remarks>
    [UProperty(
        PropertyFlags.EditDefaultsOnly | PropertyFlags.BlueprintReadOnly | PropertyFlags.Config,
        Category = "Pbs"
    )]
    [ClampMin("1")]
    [UIMax("1")]
    [Categories(IdentifierConstants.PocketTag)]
    public IReadOnlyDictionary<int, FGameplayTag> PocketNumberToGameplayTag { get; }

    /// <inheritdoc />
    protected override FText GetSectionText()
    {
        return FText.Localized("PokemonEditor", "PokemonEditor", "Pokémon Editor");
    }
}
