using Pokemon.Core.Entities;
using Pokemon.Data.Model.HardCoded;
using Pokemon.Data.Model.PBS;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.Core;
using UnrealSharp.CoreUObject;
using UnrealSharp.Engine;
using UnrealSharp.Paper2D;
using UnrealSharp.Simple2D;

namespace Pokemon.Assets;

/// <summary>
/// Represents the primary asset types for Pokémon graphics, supporting both regular and shiny forms.
/// </summary>
/// <remarks>
/// This struct is used to organize and differentiate between regular and shiny variants of Pokémon assets,
/// such as front sprites, back sprites, and icons. It is utilized by Pokémon asset loading functions
/// to resolve the correct asset based on the required type.
/// </remarks>
/// <param name="Regular">
/// The primary asset type representing the regular version of the Pokémon graphic.
/// </param>
/// <param name="Shiny">
/// The primary asset type representing the shiny version of the Pokémon graphic.
/// </param>
public readonly record struct FPokemonAssetTypes(FPrimaryAssetType Regular, FPrimaryAssetType Shiny);

/// <summary>
/// Provides functionality for resolving Pokémon asset references such as front sprites, back sprites, and icons.
/// </summary>
/// <remarks>
/// This class serves as a library for loading and resolving Pokémon-related assets based on various parameters,
/// including species, form, gender, shiny status, and shadow status. It is designed for use within Unreal Engine
/// Blueprint systems as well as C# runtime logic.
/// </remarks>
[UClass]
public class UPokemonAssetLoader : UBlueprintFunctionLibrary
{
    /// <summary>
    /// Resolves the front sprite asset for a given Pokémon species based on the specified parameters.
    /// </summary>
    /// <param name="species">The species handle of the Pokémon to resolve the sprite for.</param>
    /// <param name="form">The specific form of the Pokémon, if any. Defaults to the base form if not specified.</param>
    /// <param name="gender">The gender of the Pokémon. Defaults to Male.</param>
    /// <param name="isShiny">Indicates whether the Pokémon is shiny. Defaults to false.</param>
    /// <param name="isShadow">Indicates whether the Pokémon is shadow. Defaults to false.</param>
    /// <return>
    /// A soft object pointer to the resolved front sprite {@code USimpleFlipbook}.
    /// </return>
    [UFunction(FunctionFlags.BlueprintCallable, Category = "Graphics|Pokémon")]
    public static TSoftObjectPtr<USimpleFlipbook> ResolveSpeciesFrontSprite(
        FSpeciesHandle species,
        FName form = default,
        EPokemonGender gender = EPokemonGender.Male,
        bool isShiny = false,
        bool isShadow = false
    )
    {
        return ResolvePokemonGraphic<USimpleFlipbook>(
            new FPokemonAssetTypes(AssetTypes.PokemonFrontSprites, AssetTypes.PokemonFrontSpritesShiny),
            species,
            form,
            gender,
            isShiny,
            isShadow
        );
    }

    /// <summary>
    /// Resolves the front sprite asset for a given Pokémon instance.
    /// </summary>
    /// <param name="pokemon">The Pokémon instance for which the front sprite is to be resolved.</param>
    /// <return>
    /// A soft object pointer to the resolved front sprite of type <c>USimpleFlipbook</c>.
    /// </return>
    [UFunction(FunctionFlags.BlueprintCallable, Category = "Graphics|Pokémon")]
    public static TSoftObjectPtr<USimpleFlipbook> ResolvePokemonFrontSprite(UPokemon pokemon)
    {
        return ResolveSpeciesFrontSprite(pokemon.Species, FName.None, pokemon.Gender, pokemon.IsShiny);
    }

    /// <summary>
    /// Resolves the back sprite asset for a given Pokémon species based on the specified parameters.
    /// </summary>
    /// <param name="species">The species handle of the Pokémon to resolve the sprite for.</param>
    /// <param name="form">The specific form of the Pokémon, if any. Defaults to the base form if not specified.</param>
    /// <param name="gender">The gender of the Pokémon. Defaults to Male.</param>
    /// <param name="isShiny">Indicates whether the Pokémon is shiny. Defaults to false.</param>
    /// <param name="isShadow">Indicates whether the Pokémon is shadow. Defaults to false.</param>
    /// <return>
    /// A soft object pointer to the resolved back sprite of type USimpleFlipbook.
    /// </return>
    [UFunction(FunctionFlags.BlueprintCallable, Category = "Graphics|Pokémon")]
    public static TSoftObjectPtr<USimpleFlipbook> ResolveSpeciesBackSprite(
        FSpeciesHandle species,
        FName form = default,
        EPokemonGender gender = EPokemonGender.Male,
        bool isShiny = false,
        bool isShadow = false
    )
    {
        return ResolvePokemonGraphic<USimpleFlipbook>(
            new FPokemonAssetTypes(AssetTypes.PokemonBackSprites, AssetTypes.PokemonBackSpritesShiny),
            species,
            form,
            gender,
            isShiny,
            isShadow
        );
    }

    /// <summary>
    /// Resolves the back sprite asset for a given Pokémon instance.
    /// </summary>
    /// <param name="pokemon">The Pokémon instance for which the back sprite should be resolved.</param>
    /// <return>
    /// A soft object pointer to the resolved back sprite of type USimpleFlipbook.
    /// </return>
    [UFunction(FunctionFlags.BlueprintCallable, Category = "Graphics|Pokémon")]
    public static TSoftObjectPtr<USimpleFlipbook> ResolvePokemonBackSprite(UPokemon pokemon)
    {
        return ResolveSpeciesBackSprite(pokemon.Species, FName.None, pokemon.Gender, pokemon.IsShiny);
    }

    /// <summary>
    /// Resolves the icon asset for a given Pokémon species based on the specified parameters.
    /// </summary>
    /// <param name="species">The species handle of the Pokémon to resolve the icon for.</param>
    /// <param name="form">The specific form of the Pokémon, if any. Defaults to the base form if not specified.</param>
    /// <param name="gender">The gender of the Pokémon. Defaults to Male.</param>
    /// <param name="isShiny">Indicates whether the Pokémon is shiny. Defaults to false.</param>
    /// <param name="isShadow">Indicates whether the Pokémon is shadow. Defaults to false.</param>
    /// <return>
    /// A soft object pointer to the resolved icon asset of type <c>UPaperFlipbook</c>.
    /// </return>
    [UFunction(FunctionFlags.BlueprintCallable, Category = "Graphics|Pokémon")]
    public static TSoftObjectPtr<UPaperFlipbook> ResolveSpeciesIcon(
        FSpeciesHandle species,
        FName form = default,
        EPokemonGender gender = EPokemonGender.Male,
        bool isShiny = false,
        bool isShadow = false
    )
    {
        return ResolvePokemonGraphic<UPaperFlipbook>(
            new FPokemonAssetTypes(AssetTypes.PokemonIcons, AssetTypes.PokemonIconsShiny),
            species,
            form,
            gender,
            isShiny,
            isShadow
        );
    }

    /// <summary>
    /// Resolves the icon asset for the given Pokémon instance based on its species and attributes.
    /// </summary>
    /// <param name="pokemon">The Pokémon instance for which to resolve the icon.</param>
    /// <return>
    /// A soft object pointer to the resolved icon asset represented as {@code UPaperFlipbook}.
    /// </return>
    [UFunction(FunctionFlags.BlueprintCallable, Category = "Graphics|Pokémon")]
    public static TSoftObjectPtr<UPaperFlipbook> ResolvePokemonIcon(UPokemon pokemon)
    {
        return ResolveSpeciesIcon(pokemon.Species, FName.None, pokemon.Gender, pokemon.IsShiny);
    }

    private static TSoftObjectPtr<T> ResolvePokemonGraphic<T>(
        FPokemonAssetTypes type,
        FSpeciesHandle species,
        FName form = default,
        EPokemonGender gender = EPokemonGender.Male,
        bool isShiny = false,
        bool isShadow = false
    )
        where T : UObject
    {
        var assetManager = UAssetManager.Get();

        var factors = new List<(int priority, string value, string defaultValue)>
        {
            (0, species.ToString(), "000"),
            (1, !form.IsNone ? $"_{form}" : "", ""),
            (2, gender == EPokemonGender.Female ? "_female" : "", ""),
            (3, isShadow ? "_shadow" : "", ""),
        };

        var components = factors
            .Where(x => x.value != x.defaultValue || x.priority == 0) // Always include species
            .ToArray();

        var combinations = Enumerable
            .Range(0, 1 << components.Length)
            .Select(i =>
                (
                    i,
                    assetName: string.Concat(
                        components.Select((comp, j) => (i & (1 << j)) == 0 ? comp.value : comp.defaultValue)
                    )
                )
            )
            .Select(t => t.assetName)
            .ToArray();

        var assetTypes = isShiny ? new[] { type.Shiny, type.Regular } : new[] { type.Regular };

        foreach (var assetType in assetTypes)
        {
            foreach (var assetName in combinations)
            {
                var asset = assetManager.GetSoftObjectReferenceFromPrimaryAssetId(
                    new FPrimaryAssetId(assetType, assetName)
                );
                if (!asset.IsNull)
                    return asset.Cast<T>();
            }
        }

        return new TSoftObjectPtr<T>();
    }
}
