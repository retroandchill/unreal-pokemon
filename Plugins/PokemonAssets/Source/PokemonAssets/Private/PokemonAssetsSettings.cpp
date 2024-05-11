// "Unreal Pokémon" created by Retro & Chill.

#include "PokemonAssetsSettings.h"

const FString &UPokemonAssetsSettings::GetPokemonSpritePackageName() const {
    return PokemonSpritePackageName;
}

const FString &UPokemonAssetsSettings::GetTrainerSpritesPackageName() const {
    return TrainerSpritesPackageName;
}

const FString & UPokemonAssetsSettings::GetTypeIconsPackageName() const {
    return TypeIconsPackageName;
}

const FString & UPokemonAssetsSettings::GetStatusIconsPackageName() const {
    return StatusIconsPackageName;
}

const FString & UPokemonAssetsSettings::GetSummaryBallPackageName() const {
    return SummaryBallPackageName;
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

FSoftObjectPath UPokemonAssetsSettings::GetTypeIconRepository() const {
    return TypeIconRepository;
}

FSoftObjectPath UPokemonAssetsSettings::GetStatusIconRepository() const {
    return StatusIconRepository;
}

FSoftObjectPath UPokemonAssetsSettings::GetSummaryBallRepository() const {
    return SummaryBallRepository;
}
