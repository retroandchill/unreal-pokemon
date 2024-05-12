// "Unreal Pokémon" created by Retro & Chill.


#include "Settings/PokemonSettings.h"

FName UPokemonSettings::GetHPStat() const {
    return HPStat;
}

int32 UPokemonSettings::GetMaxMoves() const {
    return MaxMoves;
}

uint32 UPokemonSettings::GetShinyChance() const {
    return ShinyChance;
}

FName UPokemonSettings::GetDefaultPokeBall() const {
    return DefaultPokeBall;
}