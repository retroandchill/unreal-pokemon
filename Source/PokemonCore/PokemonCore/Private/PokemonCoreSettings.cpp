// "Unreal Pokémon" created by Retro & Chill.


#include "PokemonCoreSettings.h"

#include "Pokemon/Stats/StatBlock.h"

FName UPokemonCoreSettings::GetHPStat() const {
	return HPStat;
}

int32 UPokemonCoreSettings::GetMaxPartySize() const {
	return MaxPartySize;
}

TSubclassOf<UObject> UPokemonCoreSettings::GetPokemonClass() const {
	return PokemonClass;
}

FName UPokemonCoreSettings::GetStatBlockClass() const {
	return StatBlockClass;
}

TArray<FName> UPokemonCoreSettings::GetStatBlockClassOptions() {
	return FStatBlockRegistry::GetInstance().GetAllRegisteredTypes();
}
