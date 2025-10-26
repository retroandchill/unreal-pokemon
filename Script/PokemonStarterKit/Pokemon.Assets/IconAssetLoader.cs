using Pokemon.Data.Model.HardCoded;
using Pokemon.Data.Model.PBS;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.CoreUObject;
using UnrealSharp.Engine;

namespace Pokemon.Assets;

/// <summary>
/// Provides utility methods for loading and resolving icon assets within the Pokémon application.
/// Specifically, it resolves soft references to texture assets used for items and status effects.
/// </summary>
[UClass]
public partial class UIconAssetLoader : UBlueprintFunctionLibrary
{
    /// <summary>
    /// Resolves the appropriate item icon soft reference for the given item.
    /// Attempts to fetch a texture asset associated with the specified item.
    /// If no specific icon is found, a default icon is returned.
    /// </summary>
    /// <param name="item">The handle of the item for which to resolve the icon.</param>
    /// <returns>
    /// A soft object pointer to a texture representing the item's icon.
    /// If the specific icon is not found, a soft object pointer to a default texture is returned.
    /// </returns>
    [UFunction(FunctionFlags.BlueprintCallable, Category = "Graphics|Pokémon")]
    public static TSoftObjectPtr<UTexture2D> ResolveItemIcon(FItemHandle item)
    {
        var assetManager = UAssetManager.Get();
        var itemIcon = assetManager.GetSoftObjectReferenceFromPrimaryAssetId(
            new FPrimaryAssetId(AssetTypes.ItemIcons, item.ID)
        );
        return !itemIcon.IsNull
            ? itemIcon.Cast<UTexture2D>()
            : assetManager.GetSoftObjectReferenceFromPrimaryAssetId(AssetIds.ItemIcons_000).Cast<UTexture2D>();
    }

    /// <summary>
    /// Resolves the appropriate status effect icon soft reference for the given status.
    /// Retrieves a texture asset associated with the specified status.
    /// If no specific icon is found for the status, a default or generic logic may be applied depending on the implementation (not outlined here).
    /// </summary>
    /// <param name="status">The handle of the status for which to resolve the icon.</param>
    /// <returns>
    /// A soft object pointer to a texture representing the status's icon.
    /// The result may default if no specific reference is found for the provided status.
    /// </returns>
    [UFunction(FunctionFlags.BlueprintCallable, Category = "Graphics|Pokémon")]
    public static TSoftObjectPtr<UTexture2D> ResolveStatusEffectIcon(FStatusHandle status)
    {
        var assetManager = UAssetManager.Get();
        return assetManager
            .GetSoftObjectReferenceFromPrimaryAssetId(new FPrimaryAssetId(AssetTypes.StatusIcons, status.ID))
            .Cast<UTexture2D>();
    }
}
