// "Unreal Pokémon" created by Retro & Chill.

#include "Repositories/StaticImageRepository.h"
#include "Slate/SlateTextureAtlasInterface.h"

bool UStaticImageRepository::AssetValid(const FAssetData &AssetData) const {
    auto AssetClass = AssetData.GetClass(EResolveClass::Yes);
    return AssetClass->IsChildOf(UTexture2D::StaticClass()) ||
           AssetClass->ImplementsInterface(USlateTextureAtlasInterface::StaticClass()) ||
           AssetClass->IsChildOf(UMaterialInterface::StaticClass());
}

TMap<FName, TSoftObjectPtr<>> &UStaticImageRepository::GetAssetMap() {
    return AssetMap;
}

const TMap<FName, TSoftObjectPtr<>> &UStaticImageRepository::GetAssetMap() const {
    return AssetMap;
}

FStringView UStaticImageRepository::GetNamePrefix() const {
    return NamePrefix;
}