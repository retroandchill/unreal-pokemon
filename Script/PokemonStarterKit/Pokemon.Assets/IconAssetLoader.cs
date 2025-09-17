using Pokemon.Data.Model.HardCoded;
using Pokemon.Data.Model.PBS;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.CoreUObject;
using UnrealSharp.Engine;

namespace Pokemon.Assets;

[UClass]
public class UIconAssetLoader : UBlueprintFunctionLibrary
{
    [UFunction(FunctionFlags.BlueprintCallable, Category = "Graphics|Pokémon")]
    public static TSoftObjectPtr<UTexture2D> ResolveItemIcon(FItemHandle item)
    {
        var assetManager = UAssetManager.Get();
        var itemIcon = assetManager.GetSoftObjectReferenceFromPrimaryAssetId(new FPrimaryAssetId(AssetTypes.ItemIcons, item.ID));
        return !itemIcon.IsNull ? itemIcon.Cast<UTexture2D>() : assetManager.GetSoftObjectReferenceFromPrimaryAssetId(AssetIds.ItemIcons_000).Cast<UTexture2D>();
    }
    
    [UFunction(FunctionFlags.BlueprintCallable, Category = "Graphics|Pokémon")]
    public static TSoftObjectPtr<UTexture2D> ResolveStatusEffectIcon(FStatusHandle status)
    {
        var assetManager = UAssetManager.Get();
        return assetManager.GetSoftObjectReferenceFromPrimaryAssetId(new FPrimaryAssetId(AssetTypes.StatusIcons, status.ID))
            .Cast<UTexture2D>();
    }
}