// "Unreal Pokémon" created by Retro & Chill.

#include "PokemonDataSettings.h"

const TSet<FSoftObjectPath> &UPokemonDataSettings::GetDataTables() const {
    return DataTables;
}
