// "Unreal Pokémon" created by Retro & Chill.


#include "Player/Sorting/BagSorter.h"
#include <functional>

IBagSorter::~IBagSorter() = default;

void IBagSorter::SortPocket(TArrayView<FItemSlot> Pocket) {
    Pocket.Sort(std::bind_front(&IBagSorter::CompareItems, this));
}