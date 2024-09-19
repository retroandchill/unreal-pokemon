// "Unreal Pokémon" created by Retro & Chill.


#include "Assets/AssetLoader.h"
#include "Assets/AssetLoadingSettings.h"
#include "Assets/AssetUtilities.h"
#include "Ranges/Optional/GetPtrOrNull.h"

EAssetLoadResult UAssetLoader::LookupAssetByName(UClass *AssetClass, const FDirectoryPath &BasePackageName, const FString &AssetName, UObject*& FoundAsset) {
    FoundAsset = LookupAssetByName(BasePackageName, AssetName) |
        UE::Optionals::Filter([&AssetClass](const UObject &Object) { return Object.IsA(AssetClass); }) |
        UE::Optionals::GetPtrOrNull;
    return FoundAsset != nullptr ? EAssetLoadResult::Found : EAssetLoadResult::NotFound;
}

EAssetLoadResult UAssetLoader::LoadDynamicAsset(FName Identifier, const FString &AssetName, UObject*& FoundAsset) {
    auto Settings = GetDefault<UAssetLoadingSettings>();
    auto &AssetInfo = Settings->AssetClasses.FindChecked(Identifier);
    auto FullName = UAssetUtilities::GetFullAssetName(AssetName,
        AssetInfo.AssetPrefix.Get(TEXT("")));
    return LookupAssetByName(AssetInfo.AssetClass, AssetInfo.RootDirectory, FullName, FoundAsset);
}

EAssetLoadResult UAssetLoader::LookupBlueprintClassByName(UClass *BaseClass, const FDirectoryPath &BasePackageName,
                                                          const FString &AssetName, UClass*& FoundClass) {
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