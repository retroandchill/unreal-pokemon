// "Unreal Pokémon" created by Retro & Chill.

#include "Storage/StorageSystem.h"

DEFINE_INJECTABLE_DEPENDENCY(IStorageSystem)

FDepositResult::FDepositResult(int32 DepositedPokemonIndex, int32 BoxIndex) : BoxIndex(BoxIndex), DepositedPokemonIndex(DepositedPokemonIndex) {
}