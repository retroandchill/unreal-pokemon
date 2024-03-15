// "Unreal Pokémon" created by Retro & Chill.


#include "PokemonUISettings.h"

const FString& UPokemonUISettings::GetPokemonIconsPackageName() const {
	return PokemonIconsPackageName;
}

const TArray<TSubclassOf<UTextDisplayScreen>>& UPokemonUISettings::GetTextScreenClasses() const {
	return TextScreenClasses;
}
