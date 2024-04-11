// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "Meta/GenericStaticUniqueRegistry.h"

namespace Exp {
/**
 * Interface to represent various Exp. grow rates
 */
class POKEMONCORE_API IGrowthRate {
  public:
    virtual ~IGrowthRate() = default;

    /**
     * Gets the amount of Exp that is required to reach a certain level
     * @param Level The level in to calculate the Exp. for
     * @return The amount of Exp. required to level up
     */
    virtual int32 ExpForLevel(int32 Level) const = 0;
};

/**
 * Static registry for the growth rate types
 */
using FGrowthRateRegistry = TGenericStaticUniqueRegistry<IGrowthRate>;
}; // namespace Exp
