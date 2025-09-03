// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "Pokemon/Exp/GrowthRate.h"

namespace StatUtils {
    /**
     * Randomize the Pokémon's IVs and return a map of the values
     * @return The randomized IVs
     */
    POKEMONCORE_API TMap<FName, int32> RandomizeIVs();

    /**
     * Randomize the Pokémon's IV for a single stat
     * @return The randomized IV
     */
    POKEMONCORE_API int32 RandomizeIV();

    /**
     * Set all of the EVs to 0
     * @return The map of EVs all set to 0
     */
    POKEMONCORE_API TMap<FName, int32> DefaultEVs();

    /**
     * Create a new growth rate instance with the provided key
     * @param GrowthRate The name of the growth rate to get the ID for
     * @return The created growth rate
     */
    POKEMONCORE_API TUniquePtr<Exp::IGrowthRate> CreateGrowthRate(FName GrowthRate);

    /**
     * Get the maximum achievable level for a Pokémon
     */
    POKEMONCORE_API int32 GetMaxLevel();
} // namespace StatUtils
