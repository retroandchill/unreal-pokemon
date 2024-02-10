// Created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "StatBlock.h"

/**
 * The default stat calculator, which uses the formulas from the main series games
 */
class POKEMONCORE_API FDefaultStatCalculator final : public IStatBlock {
public:
	/**
	 * Initialize a new stat block with the given level randomizing the IVs and nature
	 * @param Level The level to start at
	 */
	explicit FDefaultStatCalculator(int32 Level = 5);
	
	virtual int32 CalculateStat(const TMap<FName, int32>& BaseStats, FName Stat) const override;
	virtual int32 GetLevel() const override;
	virtual TMap<FName, int32>& GetIVs() override;
	virtual const TMap<FName, int32>& GetIVs() const override;
	virtual TMap<FName, int32>& GetEVs() override;
	virtual const TMap<FName, int32>& GetEVs() const override;
	virtual const FNature& GetNature() const override;

private:
	/**
	 * The level of the Pokémon
	 */
	int32 Level;

	/**
	 * The Pokémon's IVs
	 */
	TMap<FName, int32> IVs;

	/**
	 * The Pokémon's EVs
	 */
	TMap<FName, int32> EVs;

	/**
	 * The Pokémon's Nature ID
	 */
	FName Nature;
};
