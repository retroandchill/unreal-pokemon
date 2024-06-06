// "Unreal Pokémon" created by Retro & Chill.

#include "PokemonDataSettings.h"

UPokemonDataSettings::UPokemonDataSettings() {
    PocketNames.KeySort([](uint8 A, uint8 B) { return B < A; });
}

const TSet<FSoftObjectPath> &UPokemonDataSettings::GetDataTables() const {
    return DataTables;
}

int32 UPokemonDataSettings::GetMaxDefaultAbilities() const {
    return MaxDefaultAbilities;
}

const TMap<uint8, FName> &UPokemonDataSettings::GetPocketNames() const {
    return PocketNames;
}
