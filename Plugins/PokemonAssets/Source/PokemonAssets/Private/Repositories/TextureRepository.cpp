// "Unreal Pokémon" created by Retro & Chill.

#include "Repositories/TextureRepository.h"

TMap<FName, TSoftObjectPtr<UTexture2D>> &UTextureRepository::GetAssetMap() {
    return TextureMap;
}

const TMap<FName, TSoftObjectPtr<UTexture2D>> &UTextureRepository::GetAssetMap() const {
    return TextureMap;
}

FStringView UTextureRepository::GetNamePrefix() const {
    return NamePrefix;
}