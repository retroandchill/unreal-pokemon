// "Unreal Pokémon" created by Retro & Chill.


#include "PokemonCoreSettings.h"

#include "Pokemon/Pokemon.h"

FName UPokemonCoreSettings::GetHPStat() const {
	return HPStat;
}

int32 UPokemonCoreSettings::GetMaxPartySize() const {
	return MaxPartySize;
}

FName UPokemonCoreSettings::GetPokemonClass() const {
	return PokemonClass;
}

FName UPokemonCoreSettings::GetStatBlockClass() const {
	return StatBlockClass;
}

TArray<FName> UPokemonCoreSettings::GetPokemonClassOptions() {
	return FPokemonRegistry::GetInstance().GetAllRegisteredTypes();
}

TArray<FName> UPokemonCoreSettings::GetStatBlockClassOptions() {
	return FStatBlockRegistry::GetInstance().GetAllRegisteredTypes();
}
