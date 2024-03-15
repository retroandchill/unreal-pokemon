// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "Meta/MetatypeDeclares.h"

struct FStat;
struct FNature;

/**
 * Represents an individual stat entry on a Pokémon
 */
class POKEMONCORE_API IStatEntry {
	DECLARE_ABSTRACT_METATYPE
	
public:
	virtual ~IStatEntry() = default;

	/**
	 * Get the value of this given stat in question
	 * @return The value of the stat
	 */
	virtual int32 GetStatValue() const = 0;

	/**
	 * Recalculate the value of the stat given the following information
	 * @param Level The level of the Pokémon
	 * @param Base The base stat of the Pokémon in question
	 * @param Nature The Pokémon's effective nature
	 */
	virtual void RefreshValue(int32 Level, int32 Base, const FNature& Nature) = 0;

	/**
	 * Get the information about the stat in question
	 * @return The stat's specific information
	 */
	virtual const FStat& GetStat() const = 0;

	/**
	 * Get the ID of the stat in question
	 * @return The ID of the stat in question
	 */
	virtual FName GetStatID() const = 0;

	/**
	 * Get the IV of the stat
	 * @return The value of the Pokémon's IV
	 */
	virtual int32 GetIV() const = 0;

	/**
	 * Get the EV of the stat
	 * @return The value of the Pokémon's EV
	 */
	virtual int32 GetEV() const = 0;

	/**
	 * Create a unique copy of this object
	 * @return A unique copy of this object
	 */
	virtual TUniquePtr<IStatEntry> Clone() const = 0;

	/**
	 * Compare this stat entry to another one
	 * @param Other The other stat entry
	 * @return Are the two objects equal?
	 */
	bool operator==(const IStatEntry& Other) const;

	/**
	 * Compare this stat entry to another one
	 * @param Other The other stat entry
	 * @return Are the two objects equal?
	 */
	virtual bool Equals(const IStatEntry& Other) const = 0;
};
