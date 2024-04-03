// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "StatBlockDTO.h"
#include "StatEntry.h"
#include "StatBlock.generated.h"

class IPokemon;

// This class does not need to be modified.
UINTERFACE(NotBlueprintable)
class UStatBlock : public UInterface {
	GENERATED_BODY()
};

/**
 * Represents a the stat block for calculating the Pokémon's Stats
 */
class POKEMONCORE_API IStatBlock {
	GENERATED_BODY()
	
public:
	/**
	 * Initialize a new stat block with the following initializing information
	 * @param NewOwner The Pokémon that owns this stat block
	 * @param DTO The DTO for the stat block
	 */
	virtual void Initialize(const TScriptInterface<IPokemon>& NewOwner, const FStatBlockDTO &DTO) = 0;
	
	/**
	 * Get the level of the Pokémon in question
	 * @return The level of this particular Pokémon
	 */
	UFUNCTION(BlueprintCallable, Category = Stats)
	virtual int32 GetLevel() const = 0;

	/**
	 * Get the current amount of Exp. this Pokémon has
	 * @return The current amount of Exp.
	 */
	UFUNCTION(BlueprintCallable, Category = Stats)
	virtual int32 GetExp() const = 0;

	/**
	 * Get the amount of Exp. required to reach the next level
	 * @return The exp amount required to reach the next level
	 */
	UFUNCTION(BlueprintCallable, Category = Stats)
	virtual int32 GetExpForNextLevel() const = 0;

	/**
	 * Get the Pokémon's Nature value
	 * @return The Nature of the Pokémon in question
	 */
	UFUNCTION(BlueprintCallable, Category = Stats)
	virtual const FNature& GetNature() const = 0;

	/**
	 * Get the stat that corresponds to the given ID
	 * @param Stat The stat ID to retrieve
	 * @return The entry of the stat
	 */
	virtual TScriptInterface<IStatEntry> GetStat(FName Stat) = 0;

	/**
	 * Get the stat that corresponds to the given ID
	 * @param Stat The stat ID to retrieve
	 * @return The entry of the stat
	 */
	UFUNCTION(BlueprintCallable, Category = Stats)
	virtual TScriptInterface<IStatEntry> GetStat(FName Stat) const = 0;

	/**
	 * Iterate over the stats performing the given predicate
	 * @param Predicate The function to perform on each iteration
	 */
	virtual void ForEachStat(TFunctionRef<void(FName, const IStatEntry&)> Predicate) const = 0;

	/**
	 * Calculate the stats of the given Pokémon in question
	 * @param BaseStats The base stats of the Pokémon species/form
	 */
	UFUNCTION(BlueprintCallable, Category = Stats)
	virtual void CalculateStats(const TMap<FName, int32>& BaseStats) = 0;

	/**
	 * Convert this object to its corresponding DTO
	 * @return The created DTO
	 */
	UFUNCTION(BlueprintCallable, Category = Meta)
	virtual FStatBlockDTO ToDTO() const = 0;

	/**
	 * Check if the two stat blocks are the same
	 * @param Other The other stat block
	 * @return Are these two stat blocks the same?
	 */
	UFUNCTION(BlueprintCallable, Category = Meta)
	virtual bool Equals(const TScriptInterface<IStatBlock>& Other) const = 0;
};

/**
 * Creat the stat block based on the settings in the
 * @param Outer The owner of the created object
 * @param Owner The Growth Rate type of the Pokémon
 * @param PersonalityValue The Pokémon's personality value
 * @param DTO The DTO to copy any saved data from
 * @return The created block
 */
POKEMONCORE_API TScriptInterface<IStatBlock> CreateStatBlock(const TScriptInterface<IPokemon>& Owner, const FStatBlockDTO &DTO);