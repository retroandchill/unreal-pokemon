// "Unreal Pokémon" created by Retro & Chill.


#include "PokemonCoreSettings.h"

FName UPokemonCoreSettings::GetHPStat() const {
	return HPStat;
}

int32 UPokemonCoreSettings::GetMaxPartySize() const {
	return MaxPartySize;
}
