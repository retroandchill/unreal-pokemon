// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "StatEntry.h"
#include "Species/Nature.h"

/**
 * Represents a the stat block for calculating the Pokémon's Stats
 */
class POKEMONCORE_API IStatBlock {
public:
	virtual ~IStatBlock() = default;

	/**
	 * Get the level of the Pokémon in question
	 * @return The level of this particular Pokémon
	 */
	virtual int32 GetLevel() const = 0;

	/**
	 * Get the current amount of Exp. this Pokémon has
	 * @return The current amount of Exp.
	 */
	virtual int32 GetExp() const = 0;

	/**
	 * Get the amount of Exp. required to reach the next level
	 * @return The exp amount required to reach the next level
	 */
	virtual int32 GetExpForNextLevel() const = 0;

	/**
	 * Get the Pokémon's Nature value
	 * @return The Nature of the Pokémon in question
	 */
	virtual const FNature &GetNature() const = 0;

	/**
	 * Get the stat that corresponds to the given ID
	 * @param Stat The stat ID to retrieve
	 * @return The entry of the stat
	 */
	virtual IStatEntry &GetStat(FName Stat) = 0;

	/**
	 * Get the stat that corresponds to the given ID
	 * @param Stat The stat ID to retrieve
	 * @return The entry of the stat
	 */
	virtual const IStatEntry &GetStat(FName Stat) const = 0;

	/**
	 * Calculate the stats of the given Pokémon in question
	 * @param BaseStats The base stats of the Pokémon species/form
	 */
	virtual void CalculateStats(const TMap<FName, int32> &BaseStats) = 0;
	
};
