// "Unreal Pokémon" created by Retro & Chill.

#include "Graphics/SpriteMaterials.h"
#include "Settings/SettingsStructs.h"

FPokemonSpriteMaterials::FPokemonSpriteMaterials() = default;

FPokemonSpriteMaterials::FPokemonSpriteMaterials(const FPokemonSpriteSettings &Settings)
    : BattleSpritesMaterial(Settings.BattleSpritesMaterial), UISpritesMaterial(Settings.UISpritesMaterial),
      IconMaterial(Settings.IconMaterial) {
}

FPokemonSpriteMaterials &FPokemonSpriteMaterials::operator=(const FPokemonSpriteSettings &Settings) {
    BattleSpritesMaterial = Settings.BattleSpritesMaterial;
    UISpritesMaterial = Settings.UISpritesMaterial;
    IconMaterial = Settings.IconMaterial;
    return *this;
}

FTrainerSpriteMaterials::FTrainerSpriteMaterials() = default;

FTrainerSpriteMaterials::FTrainerSpriteMaterials(const FTrainerSpriteSettings &Settings)
    : FrontSpriteBaseMaterialBattle(Settings.FrontSpriteBaseMaterialBattle),
      FrontSpriteBaseMaterialUI(Settings.FrontSpriteBaseMaterialUI) {
}

FTrainerSpriteMaterials &FTrainerSpriteMaterials::operator=(const FTrainerSpriteSettings &Settings) {
    FrontSpriteBaseMaterialBattle = Settings.FrontSpriteBaseMaterialBattle;
    FrontSpriteBaseMaterialUI = Settings.FrontSpriteBaseMaterialUI;
    return *this;
}