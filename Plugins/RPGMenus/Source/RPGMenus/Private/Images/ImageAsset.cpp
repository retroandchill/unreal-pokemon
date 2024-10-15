// "Unreal Pokémon" created by Retro & Chill.

#include "Images/ImageAsset.h"

UE_DEFINE_VARIANT_OBJECT_STRUCT(FImageAsset);

static UScriptStruct *StaticGetBaseStructureInternal(FName Name) {
    static const auto *const CoreUObjectPkg = FindObjectChecked<UPackage>(nullptr, TEXT("/Script/RPGMenus"));
    auto Result = static_cast<UScriptStruct *>(StaticFindObjectFastInternal(
        UScriptStruct::StaticClass(), CoreUObjectPkg, Name, false, RF_NoFlags, EInternalObjectFlags::None));
    check(Result != nullptr)
    ;
    Result->SetMetaData(TEXT("VariantObject"), TEXT("true"));
    return Result;
}

UScriptStruct *TBaseStructure<FImageAsset>::Get() {
    static auto *ScriptStruct = StaticGetBaseStructureInternal(TEXT("ImageAsset"));
    return ScriptStruct;
}

UScriptStruct *TBaseStructure<FSoftImageAsset>::Get() {
    static UScriptStruct *ScriptStruct = StaticGetBaseStructureInternal(TEXT("SoftImageAsset"));
    return ScriptStruct;
}

EVariantFindResult UImageAssetHelpers::MakeImageAsset(UObject *Object, FImageAsset &AsImageAsset) {
    if (!FImageAsset::GetTypeIndex(Object).IsSet()) {
        return EVariantFindResult::CastFailed;
    }

    AsImageAsset.Set(Object);
    return EVariantFindResult::CastSucceeded;
}

FImageAsset UImageAssetHelpers::MakeImageAsset_Texture2D(UTexture2D *Texture) {
    return FImageAsset(Texture);
}

FImageAsset UImageAssetHelpers::MakeImageAsset_Texture2DDynamic(UTexture2DDynamic *Texture) {
    return FImageAsset(Texture);
}

FImageAsset UImageAssetHelpers::MakeImageAsset_Material(UMaterialInterface *Material) {
    return FImageAsset(Material);
}

FImageAsset UImageAssetHelpers::MakeImageAsset_SlateTextureAsset(
    const TScriptInterface<ISlateTextureAtlasInterface> &TextureAtlas) {
    return FImageAsset(TextureAtlas);
}

FImageAsset UImageAssetHelpers::MakeImageAsset_PaperFlipbook(UPaperFlipbook *Flipbook) {
    return FImageAsset(Flipbook);
}

UObject *UImageAssetHelpers::ConvertToObject(const FImageAsset &ImageAsset) {
    return ImageAsset.TryGet().GetPtrOrNull();
}

EVariantFindResult UImageAssetHelpers::CastToTexture2D(const FImageAsset &ImageAsset, UTexture2D *&AsTexture) {
    if (!ImageAsset.IsType<UTexture2D>()) {
        return EVariantFindResult::CastFailed;
    }

    AsTexture = &ImageAsset.Get<UTexture2D>();
    return EVariantFindResult::CastSucceeded;
}

EVariantFindResult UImageAssetHelpers::CastToTextureDynamic(const FImageAsset &ImageAsset,
                                                            UTexture2DDynamic *&AsTexture) {
    if (!ImageAsset.IsType<UTexture2DDynamic>()) {
        return EVariantFindResult::CastFailed;
    }

    AsTexture = &ImageAsset.Get<UTexture2DDynamic>();
    return EVariantFindResult::CastSucceeded;
}

EVariantFindResult UImageAssetHelpers::CastToMaterial(const FImageAsset &ImageAsset, UMaterialInterface *&AsMaterial) {
    if (!ImageAsset.IsType<UMaterialInterface>()) {
        return EVariantFindResult::CastFailed;
    }

    AsMaterial = &ImageAsset.Get<UMaterialInterface>();
    return EVariantFindResult::CastSucceeded;
}

EVariantFindResult
UImageAssetHelpers::CastToSlateTextureAtlas(const FImageAsset &ImageAsset,
                                            TScriptInterface<ISlateTextureAtlasInterface> &AsTextureAtlas) {
    if (!ImageAsset.IsType<ISlateTextureAtlasInterface>()) {
        return EVariantFindResult::CastFailed;
    }

    AsTextureAtlas = ImageAsset.Get<ISlateTextureAtlasInterface>()._getUObject();
    return EVariantFindResult::CastSucceeded;
}

EVariantFindResult UImageAssetHelpers::CastToPaperFlipbook(const FImageAsset &ImageAsset, UPaperFlipbook *&AsFlipbook) {
    if (!ImageAsset.IsType<UPaperFlipbook>()) {
        return EVariantFindResult::CastFailed;
    }

    AsFlipbook = &ImageAsset.Get<UPaperFlipbook>();
    return EVariantFindResult::CastSucceeded;
}

FSoftImageAsset UImageAssetHelpers::MakeSoftImageAsset(const FImageAsset &ImageAsset) {
    return FSoftImageAsset(ImageAsset);
}

EVariantFindResult UImageAssetHelpers::MakeSoftImageAssetFromSoftObjectPtr(const TSoftObjectPtr<> &Object,
    FSoftImageAsset &AsSoftImageAsset) {
    if (!FSoftImageAsset::GetTypeIndex(Object).IsSet()) {
        return EVariantFindResult::CastFailed;
    }

    AsSoftImageAsset.Set(Object);
    return EVariantFindResult::CastSucceeded;
}

EVariantFindResult UImageAssetHelpers::LoadSynchronous(const FSoftImageAsset &Path, FImageAsset &LoadedAsset) {
    auto Result = Path.LoadSynchronous();
    if (!Result.IsSet()) {
        return EVariantFindResult::CastFailed;
    }

    LoadedAsset = FImageAsset(*Result);
    return EVariantFindResult::CastSucceeded;
}