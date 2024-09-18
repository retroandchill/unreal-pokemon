// "Unreal Pokémon" created by Retro & Chill.


#include "Assets/AssetLoader.h"
#include "Ranges/Optional/GetPtrOrNull.h"

EAssetLoadResult UAssetLoader::LookupAssetByName(UClass *AssetClass, const FDirectoryPath &BasePackageName, const FString &AssetName, UObject*& FoundAsset) {
    FoundAsset = LookupAssetByName(BasePackageName, AssetName) |
        UE::Optionals::Filter([&AssetClass](const UObject &Object) { return Object.IsA(AssetClass); }) |
        UE::Optionals::GetPtrOrNull;
    return FoundAsset != nullptr ? EAssetLoadResult::Found : EAssetLoadResult::NotFound;
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