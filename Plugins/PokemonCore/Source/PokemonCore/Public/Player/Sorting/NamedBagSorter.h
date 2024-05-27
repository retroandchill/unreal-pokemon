// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "BagSorter.h"

/**
 * Sorter for items according to their names
 */
class POKEMONCORE_API FNamedBagSorter : public IBagSorter {
protected:
    bool CompareItems(const FItemSlot &Item1, const FItemSlot &Item2) const override;

};
