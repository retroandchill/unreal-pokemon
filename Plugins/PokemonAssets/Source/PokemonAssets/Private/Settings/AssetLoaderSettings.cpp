// "Unreal Pokémon" created by Retro & Chill.

#include "Settings/AssetLoaderSettings.h"
#include "Repositories/StaticImageRepository.h"
#include "Repositories/TextureRepository.h"

UTextureRepository *UAssetLoaderSettings::GetPokemonSpriteRepository() const {
    return CastChecked<UTextureRepository>(PokemonSpriteRepository.TryLoad());
}

UTextureRepository *UAssetLoaderSettings::GetTrainerFrontSpriteRepository() const {
    return CastChecked<UTextureRepository>(TrainerFrontSpriteRepository.TryLoad());
}

UStaticImageRepository *UAssetLoaderSettings::GetTypeIconRepository() const {
    return CastChecked<UStaticImageRepository>(TypeIconRepository.TryLoad());
}

UStaticImageRepository *UAssetLoaderSettings::GetStatusIconRepository() const {
    return CastChecked<UStaticImageRepository>(StatusIconRepository.TryLoad());
}

UStaticImageRepository *UAssetLoaderSettings::GetSummaryBallRepository() const {
    return CastChecked<UStaticImageRepository>(SummaryBallRepository.TryLoad());
}

UStaticImageRepository * UAssetLoaderSettings::GetItemIconRepository() const {
    return CastChecked<UStaticImageRepository>(ItemIconRepository.TryLoad());
}
