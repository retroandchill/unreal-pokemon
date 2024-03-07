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
	 * Set all of the EVs to 0
	 * @return The map of EVs all set to 0
	 */
	POKEMONCORE_API TMap<FName, int32> DefaultEVs();

	/**
	 * Generate a random nature for the Pokémon
	 * @return The randomized Nature for the Pokémon in question
	 */
	POKEMONCORE_API FName RandomNature();

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
}

