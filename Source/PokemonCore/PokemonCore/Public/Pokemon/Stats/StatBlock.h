// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "StatBlockDTO.h"
#include "StatEntry.h"
#include "Meta/GenericStaticUniqueRegistry.h"
#include "Meta/MetatypeDeclares.h"
#include "Meta/PokeRegistry.h"
#include "Species/Nature.h"

/**
 * Represents a the stat block for calculating the Pokémon's Stats
 */
class POKEMONCORE_API IStatBlock {
	DECLARE_ABSTRACT_METATYPE
	
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
	virtual const FNature& GetNature() const = 0;

	/**
	 * Get the stat that corresponds to the given ID
	 * @param Stat The stat ID to retrieve
	 * @return The entry of the stat
	 */
	virtual IStatEntry& GetStat(FName Stat) = 0;

	/**
	 * Get the stat that corresponds to the given ID
	 * @param Stat The stat ID to retrieve
	 * @return The entry of the stat
	 */
	virtual const IStatEntry& GetStat(FName Stat) const = 0;

	/**
	 * Iterate over the stats performing the given predicate
	 * @param Predicate The function to perform on each iteration
	 */
	virtual void ForEachStat(TFunctionRef<void(FName, const IStatEntry&)> Predicate) const = 0;

	/**
	 * Calculate the stats of the given Pokémon in question
	 * @param BaseStats The base stats of the Pokémon species/form
	 */
	virtual void CalculateStats(const TMap<FName, int32>& BaseStats) = 0;

	/**
	 * Convert this object to its corresponding DTO
	 * @return The created DTO
	 */
	virtual FStatBlockDTO ToDTO() const = 0;

	/**
	 * Check if the two stat blocks are the same
	 * @param Other The other stat block
	 * @return Are these two stat blocks the same?
	 */
	virtual bool operator==(const IStatBlock& Other) const = 0;
};

using FStatBlockRegistry = TGenericStaticUniqueRegistry<IStatBlock, FName, uint32, const FStatBlockDTO&>;

/**
 * Creat the stat block based on the settings in the 
 * @param GrowthRate The Growth Rate type of the Pokémon
 * @param PersonalityValue The Pokémon's personality value
 * @param DTO The DTO to copy any saved data from
 * @return The created block
 */
POKEMONCORE_API TUniquePtr<IStatBlock> CreateStatBlock(FName GrowthRate, uint32 PersonalityValue, const FStatBlockDTO &DTO);