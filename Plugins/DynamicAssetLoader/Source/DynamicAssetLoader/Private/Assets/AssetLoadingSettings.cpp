// "Unreal Pokémon" created by Retro & Chill.

#include "Assets/AssetLoadingSettings.h"

UE_DEFINE_VARIANT_OBJECT_STRUCT(FAssetClassType);

static UScriptStruct *StaticGetBaseStructureInternal(FName Name) {
    static const auto *const CoreUObjectPkg = FindObjectChecked<UPackage>(nullptr, TEXT("/Script/DynamicAssetLoader"));
    auto Result = static_cast<UScriptStruct *>(StaticFindObjectFastInternal(
        UScriptStruct::StaticClass(), CoreUObjectPkg, Name, false, RF_NoFlags, EInternalObjectFlags::None));
    check(Result != nullptr)
    ;
    return Result;
}

UScriptStruct *TBaseStructure<FAssetClassType>::Get() {
    static auto ScriptStruct = StaticGetBaseStructureInternal(TEXT("AssetClassType"));
    return ScriptStruct;
}

UScriptStruct *TBaseStructure<FSoftAssetClassType>::Get() {
    static UScriptStruct *ScriptStruct = StaticGetBaseStructureInternal(TEXT("SoftAssetClassType"));
    return ScriptStruct;
}

FAssetLoadingEntry::FAssetLoadingEntry(const FDirectoryPath &RootDirectory, FStringView AssetPrefix, UClass *AssetClass)
    : RootDirectory(RootDirectory), AssetPrefix(AssetPrefix.IsEmpty() ? TOptional<FString>() : FString(AssetPrefix)),
      AssetClass(AssetClass), bIsNative(true) {
}

FAssetLoadingEntry::FAssetLoadingEntry(const FDirectoryPath &RootDirectory, FStringView AssetPrefix,
                                       UScriptStruct *AssetClass)
    : RootDirectory(RootDirectory), AssetPrefix(AssetPrefix.IsEmpty() ? TOptional<FString>() : FString(AssetPrefix)),
      AssetClass(AssetClass), bIsNative(true) {
}