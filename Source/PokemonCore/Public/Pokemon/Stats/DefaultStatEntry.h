// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "StatEntry.h"

/**
 * The default setup of a Pokémon's stat calculation as defined in the main series games
 */
class POKEMONCORE_API FDefaultStatEntry : public IStatEntry {
protected:
	/**
	 * Initialize the stat with the given IV and EV
	 * @param Stat The stat in question to set this to
	 * @param PersonalityValue The personality value of the owning Pokémon
	 * @param IV The IV of the stat
	 * @param EV The EV of the stat
	 */
	FDefaultStatEntry(FName Stat, uint32 PersonalityValue, const TOptional<int32>& IV, int32 EV = 0);

public:
	int32 GetStatValue() const override;
	const FStat& GetStat() const override;
	FName GetStatID() const override;
	int32 GetIV() const override;
	int32 GetEV() const override;

protected:
	/**
	 * Set the value of the stat in question
	 * @param NewValue The value of the stat
	 */
	void SetStatValue(int32 NewValue);

private:
	/**
	 * The ID of the stat in question
	 */
	FName StatID;

	/**
	 * The personality value of the owning Pokémon
	 */
	uint32 PersonalityValue;

	/**
	 * The value of the stat's IV
	 */
	int32 IV;

	/**
	 * The value of the stat's EV
	 */
	int32 EV;

	/**
	 * The actual value of the stat in question
	 */
	int32 Value = 0;
};
