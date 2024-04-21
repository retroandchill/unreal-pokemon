// "Unreal Pokémon" created by Retro & Chill.

#include "PokemonUISettings.h"

const TArray<TSubclassOf<UTextDisplayScreen>> &UPokemonUISettings::GetTextScreenClasses() const {
    return TextScreenClasses;
}
