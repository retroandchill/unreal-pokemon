// Created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Species/Nature.h"
#include "Species/Species.h"

/**
 * Abstract interface for a Pokémon object
 */
class IPokemon {
public:
	virtual ~IPokemon() = default;

	/**
	 * Get the species information about the Pokémon in question
	 * @return The species data
	 */
	virtual const FSpecies &GetSpecies() const = 0;

	/**
	 * Get the level of the Pokémon in question
	 * @return The level of this particular Pokémon
	 */
	virtual int32 GetLevel() const;

	/**
	 * Get the IV for the given stat
	 * @param Stat The stat to check the IV of
	 * @return The value of the IV
	 */
	virtual int32 GetIV(FName Stat) const;

	/**
	 * Get the EV for the given stat
	 * @param Stat The stat to check the IV of
	 * @return The value of the IV
	 */
	virtual int32 GetEV(FName Stat) const;

	/**
	 * Get the Pokémon's Nature value
	 * @return The Nature of the Pokémon in question
	 */
	virtual const FNature &GetNature() const;
};
