// "Unreal Pokémon" created by Retro & Chill.


#include "Assets/AssetLoadingSettings.h"

FAssetLoadingEntry::FAssetLoadingEntry(const FDirectoryPath &RootDirectory, FStringView AssetPrefix, UClass* AssetClass) : RootDirectory(RootDirectory), AssetPrefix(AssetPrefix.IsEmpty() ? TOptional<FString>() : FString(AssetPrefix)), AssetClass(AssetClass), bIsNative(true) {
}