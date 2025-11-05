// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Player/ItemSlot.h"
#include "RetroLib/Meta/Registry.h"

/**
 * Abstract class for handing the sorting of the player's bag.
 */
class POKEMONCORE_API IBagSorter {

  public:
    virtual ~IBagSorter();

    /**
     * Sort the provided pocket.
     * @param Pocket The pocket to sort.
     */
    void SortPocket(TArrayView<FItemSlot> Pocket) const;

  protected:
    /**
     * Compare two items and determine if the first item should go before the second.
     * @param Item1 The first item to compare
     * @param Item2 The second item to compare
     * @return Should Item1 go before Item2?
     */
    virtual bool CompareItems(const FItemSlot &Item1, const FItemSlot &Item2) const = 0;
};

DECLARE_STATIC_REGISTRY(POKEMONCORE_API, FBagSorterRegistry, Retro::Meta::TUniqueRegistry<IBagSorter>);