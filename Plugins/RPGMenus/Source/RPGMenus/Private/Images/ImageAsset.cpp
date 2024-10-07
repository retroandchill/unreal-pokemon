// "Unreal Pokémon" created by Retro & Chill.


#include "Images/ImageAsset.h"

static UScriptStruct* StaticGetBaseStructureInternal(FName Name) {
    static UPackage* CoreUObjectPkg = FindObjectChecked<UPackage>(nullptr, TEXT("/Script/RPGMenus"));
    UScriptStruct* Result = static_cast<UScriptStruct *>(StaticFindObjectFastInternal(UScriptStruct::StaticClass(),
        CoreUObjectPkg, Name, false, RF_NoFlags, EInternalObjectFlags::None));
    check(Result != nullptr);
    Result->SetMetaData(TEXT("VariantObject"), TEXT("true"));
    return Result;
}

UScriptStruct * TBaseStructure<FImageAsset>::Get() {
    static UScriptStruct* ScriptStruct = StaticGetBaseStructureInternal(TEXT("ImageAsset"));
    return ScriptStruct;
}

UScriptStruct * TBaseStructure<FSoftImageAsset>::Get() {
    static UScriptStruct* ScriptStruct = StaticGetBaseStructureInternal(TEXT("SoftImageAsset"));
    return ScriptStruct;
}

EVariantFindResult UImageAssetHelpers::MakeImageAsset(UObject *Object, FImageAsset &AsImageAsset) {
    if (!FImageAsset::GetTypeIndex(Object).IsSet()) {
        return EVariantFindResult::CastFailed;
    }

    AsImageAsset.Set(Object);
    return EVariantFindResult::CastSucceeded;
}

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