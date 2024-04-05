// "Unreal Pokémon" created by Retro & Chill.


#include "PokemonUISettings.h"

const FString& UPokemonUISettings::GetPokemonIconsPackageName() const {
	return PokemonIconsPackageName;
}

const FString& UPokemonUISettings::GetTrainerSpritesPackageName() const {
	return TrainerSpritesPackageName;
}

const FSoftObjectPath& UPokemonUISettings::GetPokemonIconsBaseMaterial() const {
	return PokemonIconsBaseMaterial;
}

FName UPokemonUISettings::GetIconSourceTexturePropertyName() const {
	return IconSourceTexturePropertyName;
}

FName UPokemonUISettings::GetIconFrameRatePropertyName() const {
	return IconFrameRatePropertyName;
}

const FSoftObjectPath& UPokemonUISettings::GetTrainerSpriteBaseMaterial() const {
	return TrainerSpriteBaseMaterial;
}

FName UPokemonUISettings::GetTrainerSpriteSourceTexturePropertyName() const {
	return TrainerSpriteSourceTexturePropertyName;
}

const TArray<TSubclassOf<UTextDisplayScreen>>& UPokemonUISettings::GetTextScreenClasses() const {
	return TextScreenClasses;
}
