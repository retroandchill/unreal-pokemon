// "Unreal Pokémon" created by Retro & Chill.

#include "PokemonCoreSettings.h"
#include "Pokemon/GamePokemon.h"
#include "Pokemon/Stats/DefaultStatBlock.h"

FName UPokemonCoreSettings::GetHPStat() const {
    return HPStat;
}

int32 UPokemonCoreSettings::GetMaxPartySize() const {
    return MaxPartySize;
}

TSubclassOf<UObject> UPokemonCoreSettings::GetPokemonClass() const {
    return PokemonClass;
}

TSubclassOf<UObject> UPokemonCoreSettings::GetStatBlockClass() const {
    return StatBlockClass;
}
