// "Unreal Pokémon" created by Retro & Chill.

#include "Images/ImageAsset.h"

UE_DEFINE_VARIANT_OBJECT_STRUCT(FImageAsset);

static UScriptStruct *StaticGetBaseStructureInternal(FName Name) {
    static const auto *const CoreUObjectPkg = FindObjectChecked<UPackage>(nullptr, TEXT("/Script/RPGMenus"));
    auto Result = static_cast<UScriptStruct *>(StaticFindObjectFastInternal(
        UScriptStruct::StaticClass(), CoreUObjectPkg, Name, false, RF_NoFlags, EInternalObjectFlags::None));
    check(Result != nullptr)
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