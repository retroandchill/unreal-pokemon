// "Unreal Pokémon" created by Retro & Chill.

#include "Player/Sorting/BagSorter.h"
#include <functional>

DEFINE_STATIC_REGISTRY(FBagSorterRegistry)

IBagSorter::~IBagSorter() = default;

void IBagSorter::SortPocket(TArrayView<FItemSlot> Pocket) const {
    Pocket.Sort(std::bind_front(&IBagSorter::CompareItems, this));
}