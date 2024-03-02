// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "Breeding/Gender.h"
#include "Species/SpeciesData.h"
#include "Stats/StatBlock.h"

/**
 * Abstract interface for a Pokémon object
 */
class POKEMONCORE_API IPokemon {
public:
	virtual ~IPokemon() = default;

	/**
	 * Get the name of the Pokémon in question
	 * @return The Pokémon's Nickname
	 */
	virtual FText GetName() const = 0;

	/**
	 * Get the species information about the Pokémon in question
	 * @return The species data
	 */
	virtual const FSpeciesData &GetSpecies() const = 0;

	/**
	 * Get the gender of the Pokémon in question
	 * @return The Pokémon's gender
	 */
	virtual EGender GetGender() const = 0;

	/**
	 * Get the current HP of the Pokémon in question
	 * @return The current HP of this particular Pokémon
	 */
	virtual int32 GetCurrentHP() const = 0;

	/**
	 * Get the current HP of the Pokémon in question
	 * @return The current HP of this particular Pokémon
	 */
	virtual int32 GetMaxHP() const = 0;

	/**
	 * Get the stat information for this Pokémon
	 * @return A reference to the owned stat block
	 */
	virtual const IStatBlock &GetStatBlock() const = 0;
	
};
