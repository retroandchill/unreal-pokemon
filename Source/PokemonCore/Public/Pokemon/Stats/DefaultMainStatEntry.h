// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "DefaultStatEntry.h"

/**
 * Represents the calculation implementation of the default main stat
 */
class POKEMONCORE_API FDefaultMainStatEntry : public FDefaultStatEntry {
	DECLARE_DERIVED_METATYPE
	
public:
	/**
	 * Initialize the stat with the given IV and EV
	 * @param Stat The stat in question to set this to
	 * @param IV The IV of the stat
	 * @param EV The EV of the stat
	 */
	FDefaultMainStatEntry(FName Stat, uint32 PersonalityValue, const TOptional<int32>& IV, int32 EV = 0);

	void RefreshValue(int32 Level, int32 Base, const FNature& Nature) final;
	TUniquePtr<IStatEntry> Clone() const override;
	
};
