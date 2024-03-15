// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "StatBlock.h"
#include "Memory/RowPointer.h"
#include "Pokemon/Exp/GrowthRate.h"

struct FStatBlockDTO;
/**
 * The default stat calculator, which uses the formulas from the main series games
 */
class POKEMONCORE_API FDefaultStatBlock : public IStatBlock {
	DECLARE_DERIVED_METATYPE
	
public:
	/**
	 * Initialize a new stat block with the following initializing information
	 * @param GrowthRateID The Exp growth rate to assign
	 * @param PersonalityValue The personality value of the owning Pokémon
	 * @param DTO The DTO for the stat block
	 */
	FDefaultStatBlock(FName GrowthRateID, uint32 PersonalityValue, const FStatBlockDTO &DTO);

	~FDefaultStatBlock() override;

	FDefaultStatBlock(const FDefaultStatBlock& Other);
	FDefaultStatBlock(FDefaultStatBlock&& Other) noexcept;

	FDefaultStatBlock& operator=(const FDefaultStatBlock& Other);
	FDefaultStatBlock& operator=(FDefaultStatBlock&& Other) noexcept;

	int32 GetLevel() const override;
	int32 GetExp() const override;
	int32 GetExpForNextLevel() const override;
	const FNature& GetNature() const override;
	IStatEntry& GetStat(FName Stat) override;
	const IStatEntry& GetStat(FName Stat) const override;
	void ForEachStat(TFunctionRef<void(FName, const IStatEntry&)> Predicate) const override;
	void CalculateStats(const TMap<FName, int32>& BaseStats) override;
	virtual FStatBlockDTO ToDTO() const override;
	
	virtual bool operator==(const IStatBlock& Other) const override;

	/**
	 * Check if the two stat blocks are the same
	 * @param Other The other stat block
	 * @return Are these two stat blocks the same?
	 */
	bool operator==(const FDefaultStatBlock& Other) const;

private:
	/**
	 * The level of the Pokémon
	 */
	int32 Level;

	/**
	 * The personality value of the owning Pokémon
	 */
	uint32 PersonalityValue;

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
	TOptional<TRowPointer<FNature>> Nature;

	/**
	 * Map to each of the Pokémon's individual stat values
	 */
	TMap<FName, TUniquePtr<IStatEntry>> Stats;
};
