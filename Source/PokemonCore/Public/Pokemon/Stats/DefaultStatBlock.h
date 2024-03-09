// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "StatBlock.h"
#include "Pokemon/Exp/GrowthRate.h"

/**
 * The default stat calculator, which uses the formulas from the main series games
 */
class POKEMONCORE_API FDefaultStatBlock : public IStatBlock {
public:
	
	/**
	 * Initialize a new stat block with the given level randomizing the IVs and nature
	 * @param GrowthRateID The Exp growth rate to assign
	 * @param Level The level to start at
	 */
	explicit FDefaultStatBlock(FName GrowthRateID, int32 Level = 5);

	/**
	 * Initialize a new stat block explicitly setting the values of all the Pokémon's stats
	 * @param GrowthRateID The Exp growth rate to assign
	 * @param Level The level to start at 
	 * @param IVs The values of the IVs
	 * @param EVs The values of the EVs
	 * @param Nature The nature of the Pokémon in question
	 */
	FDefaultStatBlock(FName GrowthRateID, int32 Level, const TMap<FName, int32> &IVs, const TMap<FName, int32> &EVs, FName Nature);

	~FDefaultStatBlock() override;
	
	FDefaultStatBlock(const FDefaultStatBlock& Other);
	FDefaultStatBlock(FDefaultStatBlock&& Other) noexcept;
	
	FDefaultStatBlock &operator=(const FDefaultStatBlock& Other);
	FDefaultStatBlock &operator=(FDefaultStatBlock&& Other) noexcept;
	
	int32 GetLevel() const override;
	int32 GetExp() const override;
	int32 GetExpForNextLevel() const override;
	const FNature& GetNature() const override;
	IStatEntry& GetStat(FName Stat) override;
	const IStatEntry& GetStat(FName Stat) const override;
	void CalculateStats(const TMap<FName, int32>& BaseStats) override;

private:
	/**
	 * The level of the Pokémon
	 */
	int32 Level;

	/**
	 * The Exp. growth rate of the Pokémon
	 */
	TUniquePtr<Exp::IGrowthRate> GrowthRate;

	/**
	 * The current Exp of this Pokémon
	 */
	int32 Exp;

	/**
	 * The Pokémon's Nature ID
	 */
	FName Nature;

	/**
	 * Map to each of the Pokémon's individual stat values
	 */
	TMap<FName, TUniquePtr<IStatEntry>> Stats;
};
