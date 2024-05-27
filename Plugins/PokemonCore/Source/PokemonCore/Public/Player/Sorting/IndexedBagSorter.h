// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "BagSorter.h"

/**
 * Sorter for items according to their internal index.
 */
class POKEMONCORE_API FIndexedBagSorter : IBagSorter {
protected:
    bool CompareItems(const FItemSlot &Item1, const FItemSlot &Item2) const override;

};
