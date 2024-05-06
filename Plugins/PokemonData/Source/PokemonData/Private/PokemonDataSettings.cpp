// "Unreal Pokémon" created by Retro & Chill.

#include "PokemonDataSettings.h"

const TSet<FSoftObjectPath> &UPokemonDataSettings::GetDataTables() const {
    return DataTables;
}

int32 UPokemonDataSettings::GetMaxDefaultAbilities() const {
    return MaxDefaultAbilities;
}
