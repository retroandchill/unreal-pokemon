// "Unreal Pokémon" created by Retro & Chill.

#include "Assets/AssetLoadingSettings.h"

static UScriptStruct* StaticGetBaseStructureInternal(FName Name) {
    static UPackage* CoreUObjectPkg = FindObjectChecked<UPackage>(nullptr, TEXT("/Script/DynamicAssetLoader"));
    UScriptStruct* Result = static_cast<UScriptStruct *>(StaticFindObjectFastInternal(UScriptStruct::StaticClass(),
        CoreUObjectPkg, Name, false, RF_NoFlags, EInternalObjectFlags::None));
    check(Result != nullptr);
    return Result;
}

UScriptStruct * TBaseStructure<FAssetClassType>::Get() {
    static UScriptStruct* ScriptStruct = StaticGetBaseStructureInternal(TEXT("AssetClassType"));
    return ScriptStruct;
}

FAssetLoadingEntry::FAssetLoadingEntry(const FDirectoryPath &RootDirectory, FStringView AssetPrefix, UClass *AssetClass)
    : RootDirectory(RootDirectory), AssetPrefix(AssetPrefix.IsEmpty() ? TOptional<FString>() : FString(AssetPrefix)),
      AssetClass(AssetClass), bIsNative(true) {
}