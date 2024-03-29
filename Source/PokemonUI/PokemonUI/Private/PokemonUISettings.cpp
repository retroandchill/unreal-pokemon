// "Unreal Pokémon" created by Retro & Chill.


#include "PokemonUISettings.h"

const FString& UPokemonUISettings::GetPokemonIconsPackageName() const {
	return PokemonIconsPackageName;
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

const TArray<TSubclassOf<UTextDisplayScreen>>& UPokemonUISettings::GetTextScreenClasses() const {
	return TextScreenClasses;
}
