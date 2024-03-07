// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "DefaultStatEntry.h"

/**
 * Handles the default representation of the main battle stats
 */
class POKEMONCORE_API FDefaultMainBattleStatEntry : public FDefaultStatEntry {
public:
	/**
	 * Initialize the stat with the given IV and EV
	 * @param Stat The stat in question to set this to
	 * @param IV The IV of the stat
	 * @param EV The EV of the stat
	 */
	FDefaultMainBattleStatEntry(FName Stat, int32 IV, int32 EV = 0);
	
	void RefreshValue(int32 Level, int32 Base, const FNature& Nature) final;
	TUniquePtr<IStatEntry> Clone() const override;
};
