// "Unreal Pokémon" created by Retro & Chill.

#include "PokemonAssetsSettings.h"

const FString &UPokemonAssetsSettings::GetPokemonSpritePackageName() const {
    return PokemonSpritePackageName;
}

const FString &UPokemonAssetsSettings::GetTrainerSpritesPackageName() const {
    return TrainerSpritesPackageName;
}

const FSoftObjectPath &UPokemonAssetsSettings::GetPokemonIconsBaseMaterial() const {
    return PokemonIconsBaseMaterial;
}

FName UPokemonAssetsSettings::GetIconSourceTexturePropertyName() const {
    return IconSourceTexturePropertyName;
}

FName UPokemonAssetsSettings::GetIconFrameRatePropertyName() const {
    return IconFrameRatePropertyName;
}

const FSoftObjectPath &UPokemonAssetsSettings::GetTrainerSpriteBaseMaterial() const {
    return TrainerSpriteBaseMaterial;
}

FName UPokemonAssetsSettings::GetTrainerSpriteSourceTexturePropertyName() const {
    return TrainerSpriteSourceTexturePropertyName;
}

FSoftObjectPath UPokemonAssetsSettings::GetPokemonSpriteRepository() const {
    return PokemonSpriteRepository;
}

FSoftObjectPath UPokemonAssetsSettings::GetTrainerFrontSpriteRepository() const {
    return TrainerFrontSpriteRepository;
}
