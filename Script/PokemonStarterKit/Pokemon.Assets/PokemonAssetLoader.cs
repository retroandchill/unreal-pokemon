using Pokemon.Core.Entities;
using Pokemon.Data.Model.HardCoded;
using Pokemon.Data.Model.PBS;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.CoreUObject;
using UnrealSharp.Engine;

namespace Pokemon.Assets;

public readonly record struct FPokemonAssetTypes(FPrimaryAssetType Regular, FPrimaryAssetType Shiny);

[UClass]
public class UPokemonAssetLoader : UBlueprintFunctionLibrary
{
    [UFunction(FunctionFlags.BlueprintCallable, Category = "Graphics|Pokémon")]
    public static TSoftObjectPtr<UObject> ResolveSpeciesFrontSprite(FSpeciesHandle species,
                                                                    FName form = default,
                                                                    EPokemonGender gender = EPokemonGender.Male,
                                                                    bool isShiny = false, bool isShadow = false)
    {
        return ResolvePokemonGraphic(new FPokemonAssetTypes(AssetTypes.PokemonFrontSprites, AssetTypes.PokemonFrontSpritesShiny), species, form, gender, isShiny, isShadow);
    }
    
    [UFunction(FunctionFlags.BlueprintCallable, Category = "Graphics|Pokémon")]
    public static TSoftObjectPtr<UObject> ResolvePokemonFrontSprite(UPokemon pokemon)
    {
        return ResolveSpeciesFrontSprite(pokemon.Species, FName.None, pokemon.Gender, pokemon.IsShiny);
    }
    
    [UFunction(FunctionFlags.BlueprintCallable, Category = "Graphics|Pokémon")]
    public static TSoftObjectPtr<UObject> ResolveSpeciesBackSprite(FSpeciesHandle species,
                                                                   FName form = default,
                                                                   EPokemonGender gender = EPokemonGender.Male,
                                                                   bool isShiny = false, bool isShadow = false)
    {
        return ResolvePokemonGraphic(new FPokemonAssetTypes(AssetTypes.PokemonBackSprites, AssetTypes.PokemonBackSpritesShiny), species, form, gender, isShiny, isShadow);
    }
    
    [UFunction(FunctionFlags.BlueprintCallable, Category = "Graphics|Pokémon")]
    public static TSoftObjectPtr<UObject> ResolvePokemonBackSprite(UPokemon pokemon)
    {
        return ResolveSpeciesBackSprite(pokemon.Species, FName.None, pokemon.Gender, pokemon.IsShiny);
    }
    
    [UFunction(FunctionFlags.BlueprintCallable, Category = "Graphics|Pokémon")]
    public static TSoftObjectPtr<UObject> ResolveSpeciesIcon(FSpeciesHandle species,
                                                             FName form = default,
                                                             EPokemonGender gender = EPokemonGender.Male,
                                                             bool isShiny = false, bool isShadow = false)
    {
        return ResolvePokemonGraphic(new FPokemonAssetTypes(AssetTypes.PokemonIcons, AssetTypes.PokemonIconsShiny), species, form, gender, isShiny, isShadow);
    }

    [UFunction(FunctionFlags.BlueprintCallable, Category = "Graphics|Pokémon")]
    public static TSoftObjectPtr<UObject> ResolvePokemonIcon(UPokemon pokemon)
    {
        return ResolveSpeciesIcon(pokemon.Species, FName.None, pokemon.Gender, pokemon.IsShiny);
    }
    
    private static TSoftObjectPtr<UObject> ResolvePokemonGraphic(FPokemonAssetTypes type, FSpeciesHandle species, 
                                                                      FName form = default, 
                                                                      EPokemonGender gender = EPokemonGender.Male, 
                                                                      bool isShiny = false, bool isShadow = false)
    {
        var assetManager = UAssetManager.Get();

        var factors = new List<(int priority, string value, string defaultValue)>
        {
            (0, species.ToString(), "000"),
            (1, !form.IsNone ? $"_{form}" : "", ""),
            (2, gender == EPokemonGender.Female ? "_female" : "", ""),
            (3, isShadow ? "_shadow" : "", "")
        };
        
        var components = factors
            .Where(x => x.value != x.defaultValue || x.priority == 0) // Always include species
            .ToArray(); 

        var combinations = Enumerable.Range(0, 1 << components.Length)
            .Select(i => (i, assetName: string.Concat(components.Select((comp, j) =>
                (i & (1 << j)) == 0 ? comp.value : comp.defaultValue))))
            .Select(t => t.assetName)
            .ToArray();

        var assetTypes = isShiny 
            ? new[] { type.Shiny, type.Regular }
            : new[] { type.Regular };

        foreach (var assetType in assetTypes)
        {
            foreach (var assetName in combinations)
            {

                var asset = assetManager.GetSoftObjectReferenceFromPrimaryAssetId(
                    new FPrimaryAssetId(assetType, assetName));
                if (!asset.IsNull)
                    return asset;
            }
        }

        return new TSoftObjectPtr<UObject>();
    }

}