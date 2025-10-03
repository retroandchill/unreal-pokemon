using UnrealSharp.CoreUObject;
using UnrealSharp.GameplayTags;
using UnrealSharp.PokemonEditorTools;

namespace Pokemon.Editor.Serialization.Pbs.Utilities;

/// <summary>
/// Provides extension methods for handling and manipulating gameplay tags.
/// </summary>
/// <remarks>
/// This static class is intended to offer utility functions and helpers
/// for working with gameplay tags in different contexts.
/// </remarks>
public static class GameplayTagExtensions
{
    /// <summary>
    /// Extensions for FGameplayTag.
    /// </summary>
    extension(FGameplayTag)
    {
        /// <summary>
        /// Retrieves an existing gameplay tag or creates a new one if it doesn't exist.
        /// </summary>
        /// <param name="tagString">The gameplay tag string to retrieve or create.</param>
        /// <returns>An instance of <see cref="FGameplayTag"/> corresponding to the provided tag string.</returns>
        /// <exception cref="InvalidOperationException">
        /// Thrown if the tag cannot be created due to an error in the tag source or configuration.
        /// </exception>
        public static FGameplayTag GetOrCreate(string tagString)
        {
            var tagSource = UObject.GetDefault<UPokemonEditorSettings>().NewGameplayTagsPath;
            return !UImportUtils.TryAddGameplayTagToIni(tagSource, tagString, out var error)
                ? throw new InvalidOperationException(error)
                : new FGameplayTag(tagString);
        }
    }
}
