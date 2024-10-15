// "Unreal Pokémon" created by Retro & Chill.

#include "Assets/AssetLoader.h"
#include "Assets/AssetLoadingSettings.h"
#include "Assets/AssetUtilities.h"
#include "Ranges/Optional/GetPtrOrNull.h"
#include "Ranges/Optional/OrElseGet.h"

FString UAssetLoader::CreateSearchKey(FStringView BasePackageName, FStringView AssetName) {
    FStringView Prefix;
    if (int32 CharIndex; AssetName.FindLastChar('/', CharIndex)) {
        int32 PrefixLength = CharIndex + 1;
        Prefix = AssetName.SubStr(0, PrefixLength);
        AssetName = AssetName.RightChop(PrefixLength);
    }

    return FString::Format(TEXT("{0}/{1}{2}.{2}"), {BasePackageName, Prefix, AssetName});
}

EAssetLoadResult UAssetLoader::FindAssetByName(const UClass *AssetClass, const FDirectoryPath &BasePackageName,
                                               const FString &AssetName, UObject *&FoundAsset) {
    FoundAsset = FindAssetByName(BasePackageName, AssetName) |
                 UE::Optionals::Filter([&AssetClass](const UObject &Object) { return Object.IsA(AssetClass); }) |
                 UE::Optionals::GetPtrOrNull;
    return FoundAsset != nullptr ? EAssetLoadResult::Found : EAssetLoadResult::NotFound;
}

EAssetLoadResult UAssetLoader::LookupAssetByName(const UClass *AssetClass, const FDirectoryPath &BasePackageName,
    const FString &AssetName, TSoftObjectPtr<> &FoundAsset) {
    auto Value = LookupAssetByName(BasePackageName, AssetName) |
                 UE::Optionals::Filter(&TSoftObjectRef<>::IsAssetOfType, AssetClass);
    // clang-format off
    FoundAsset = LookupAssetByName(BasePackageName, AssetName) |
                 UE::Optionals::Filter(&TSoftObjectRef<>::IsAssetOfType, AssetClass) |
                 UE::Optionals::Map(&TSoftObjectRef<>::ToSoftObjectPtr) |
                 UE::Optionals::OrElseGet([] { return TSoftObjectPtr(); });
    // clang-format on
    return FoundAsset.IsNull() ? EAssetLoadResult::Found : EAssetLoadResult::NotFound;
}

EAssetLoadResult UAssetLoader::LoadDynamicAsset(FName Identifier, const FString &AssetName, UObject *&FoundAsset) {
    auto Settings = GetDefault<UAssetLoadingSettings>();
    auto &AssetInfo = Settings->AssetClasses.FindChecked(Identifier);
    auto FullName = UAssetUtilities::GetFullAssetName(AssetName, AssetInfo.AssetPrefix.Get(TEXT("")));
    return FindAssetByName(&AssetInfo.AssetClass.TryGet<UClass>().Get(*UObject::StaticClass()), AssetInfo.RootDirectory,
                           FullName, FoundAsset);
}

EAssetLoadResult UAssetLoader::LookupDynamicAsset(FName Identifier, const FString &AssetName,
    TSoftObjectPtr<UObject> &FoundAsset) {
    auto Settings = GetDefault<UAssetLoadingSettings>();
    auto &AssetInfo = Settings->AssetClasses.FindChecked(Identifier);
    auto FullName = UAssetUtilities::GetFullAssetName(AssetName, AssetInfo.AssetPrefix.Get(TEXT("")));
    return LookupAssetByName(&AssetInfo.AssetClass.TryGet<UClass>().Get(*UObject::StaticClass()), AssetInfo.RootDirectory,
                           FullName, FoundAsset);
}

EAssetLoadResult UAssetLoader::LookupBlueprintClassByName(UClass *BaseClass, const FDirectoryPath &BasePackageName,
                                                          const FString &AssetName, UClass *&FoundClass) {
    FoundClass = LookupBlueprintClassByName(BasePackageName, AssetName) |
                 UE::Optionals::Filter([&BaseClass](const UClass &Class) { return Class.IsChildOf(BaseClass); }) |
                 UE::Optionals::GetPtrOrNull;
    return FoundClass != nullptr ? EAssetLoadResult::Found : EAssetLoadResult::NotFound;
}

EAssetLoadResult UAssetLoader::ResolveAsset(UClass *AssetClass, const FDirectoryPath &BasePackageName,
                                            const TArray<FString> &Keys, UObject *&FoundAsset) {
    FoundAsset = ResolveAsset(BasePackageName, Keys) |
                 UE::Optionals::Filter([&AssetClass](const UObject &Object) { return Object.IsA(AssetClass); }) |
                 UE::Optionals::GetPtrOrNull;
    return FoundAsset != nullptr ? EAssetLoadResult::Found : EAssetLoadResult::NotFound;
}

EAssetLoadResult UAssetLoader::ResolveClass(UClass *AssetClass, const FDirectoryPath &BasePackageName,
                                            const TArray<FString> &Keys, UClass *&FoundClass) {
    FoundClass = ResolveClass(BasePackageName, Keys) |
                 UE::Optionals::Filter([&AssetClass](const UObject &Object) { return Object.IsA(AssetClass); }) |
                 UE::Optionals::GetPtrOrNull;
    return FoundClass != nullptr ? EAssetLoadResult::Found : EAssetLoadResult::NotFound;
}