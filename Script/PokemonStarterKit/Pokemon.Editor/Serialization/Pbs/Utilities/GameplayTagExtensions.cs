using UnrealSharp.CoreUObject;
using UnrealSharp.GameplayTags;
using UnrealSharp.PokemonEditorTools;

namespace Pokemon.Editor.Serialization.Pbs.Utilities;

public static class GameplayTagExtensions
{
    extension(FGameplayTag)
    {
        public static FGameplayTag GetOrCreate(string tagString)
        {
            var tagSource = UObject.GetDefault<UPokemonEditorSettings>().NewGameplayTagsPath;
            return !UImportUtils.TryAddGameplayTagToIni(tagSource, tagString, out var error)
                ? throw new InvalidOperationException(error)
                : new FGameplayTag(tagString);
        }
    }
}
