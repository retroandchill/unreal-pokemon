﻿// "Unreal Pokémon" created by Retro & Chill.


#include "AssetAssignmentSettings.h"

const FString &UAssetAssignmentSettings::GetPokemonSpritePackageName() const {
    return PokemonSpritePackageName;
}

const FString &UAssetAssignmentSettings::GetTrainerSpritesPackageName() const {
    return TrainerSpritesPackageName;
}

const FString & UAssetAssignmentSettings::GetTypeIconsPackageName() const {
    return TypeIconsPackageName;
}

const FString & UAssetAssignmentSettings::GetStatusIconsPackageName() const {
    return StatusIconsPackageName;
}

const FString & UAssetAssignmentSettings::GetSummaryBallPackageName() const {
    return SummaryBallPackageName;
}