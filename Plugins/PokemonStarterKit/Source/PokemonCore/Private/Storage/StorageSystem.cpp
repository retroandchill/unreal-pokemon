// "Unreal Pokémon" created by Retro & Chill.

#include "Storage/StorageSystem.h"

// Add default functionality here for any IStorageSystem functions that are not pure virtual.
FDepositResult::FDepositResult(int32 DepositedPokemonIndex, int32 BoxIndex) : BoxIndex(BoxIndex), DepositedPokemonIndex(DepositedPokemonIndex) {
}