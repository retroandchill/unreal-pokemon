// "Unreal Pokémon" created by Retro & Chill.


#include "Images/ImageAssetHelpers.h"


FImageAsset UImageAssetHelpers::MakeImageAsset_Texture(UTexture *Texture) {
    return FImageAsset(Texture);
}

FImageAsset UImageAssetHelpers::MakeImageAsset_Material(UMaterialInterface *Material) {
    return FImageAsset(Material);
}

FImageAsset UImageAssetHelpers::MakeImageAsset_SlateTextureAsset(const TScriptInterface<ISlateTextureAtlasInterface> &TextureAtlas) {
    return FImageAsset(TextureAtlas);
}

UObject * UImageAssetHelpers::ConvertToObject(const FImageAsset &ImageAsset) {
    return ImageAsset.TryGet().GetPtrOrNull();
}

EVariantFindResult UImageAssetHelpers::CastToTexture(const FImageAsset &ImageAsset, UTexture *&AsTexture) {
    if (!ImageAsset.IsType<UTexture>()) {
        return EVariantFindResult::CastFailed;
    }
    
    AsTexture = &ImageAsset.Get<UTexture>();
    return EVariantFindResult::CastSucceeded;
}

EVariantFindResult UImageAssetHelpers::CastToMaterial(const FImageAsset &ImageAsset, UMaterialInterface *&AsMaterial) {
    if (!ImageAsset.IsType<UMaterialInterface>()) {
        return EVariantFindResult::CastFailed;
    }
    
    AsMaterial = &ImageAsset.Get<UMaterialInterface>();
    return EVariantFindResult::CastSucceeded;
}

EVariantFindResult UImageAssetHelpers::CastToSlateTextureAtlas(const FImageAsset &ImageAsset,
    TScriptInterface<ISlateTextureAtlasInterface> &AsTextureAtlas) {
    if (!ImageAsset.IsType<ISlateTextureAtlasInterface>()) {
        return EVariantFindResult::CastFailed;
    }
    
    AsTextureAtlas = ImageAsset.Get<ISlateTextureAtlasInterface>()._getUObject();
    return EVariantFindResult::CastSucceeded;
}