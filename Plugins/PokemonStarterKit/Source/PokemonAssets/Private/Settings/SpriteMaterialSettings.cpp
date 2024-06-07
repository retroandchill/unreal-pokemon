// "Unreal Pokémon" created by Retro & Chill.

#include "Settings/SpriteMaterialSettings.h"

const FPokemonSpriteSettings &USpriteMaterialSettings::GetPokemonSpriteSettings() const {
    return PokemonSprites;
}

const FTrainerSpriteSettings &USpriteMaterialSettings::GetTrainerSpriteSettings() const {
    return TrainerSprites;
}