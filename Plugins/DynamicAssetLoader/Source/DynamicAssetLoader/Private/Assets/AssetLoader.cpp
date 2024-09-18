// "Unreal Pokémon" created by Retro & Chill.


#include "Assets/AssetLoader.h"
#include "Ranges/Optional/GetPtrOrNull.h"

UObject * UAssetLoader::LookupAssetByName(UClass *AssetClass, const FDirectoryPath &BasePackageName, const FString &AssetName) {
    return LookupAssetByName(BasePackageName, AssetName) |
        UE::Optionals::Filter([&AssetClass](const UObject &Object) { return Object.IsA(AssetClass); }) |
        UE::Optionals::GetPtrOrNull;
}

UClass * UAssetLoader::LookupBlueprintClassByName(UClass *BaseClass, const FDirectoryPath &BasePackageName,
    const FString &AssetName) {
    return LookupBlueprintClassByName(BasePackageName, AssetName) |
        UE::Optionals::Filter([&BaseClass](const UClass &Class) { return Class.IsChildOf(BaseClass); }) |
        UE::Optionals::GetPtrOrNull;
}