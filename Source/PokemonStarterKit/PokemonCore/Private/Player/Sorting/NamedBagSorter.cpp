// "Unreal Pokémon" created by Retro & Chill.

#include "Player/Sorting/NamedBagSorter.h"
#include "Player/ItemSlot.h"

bool FNamedBagSorter::CompareItems(const FItemSlot &Item1, const FItemSlot &Item2) const
{
    return Item1.GetItem().RealName.ToString() < Item2.GetItem().RealName.ToString();
}