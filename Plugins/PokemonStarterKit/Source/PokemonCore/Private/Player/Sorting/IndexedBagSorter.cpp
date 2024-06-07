// "Unreal Pokémon" created by Retro & Chill.

#include "Player/Sorting/IndexedBagSorter.h"
#include "Player/ItemSlot.h"

bool FIndexedBagSorter::CompareItems(const FItemSlot &Item1, const FItemSlot &Item2) const {
    return Item1.GetItem().RowID < Item2.GetItem().RowID;
}