// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "Species/SpeciesData.h"
#include "Stats/StatBlock.h"

/**
 * Abstract interface for a Pokémon object
 */
class POKEMONCORE_API IPokemon {
public:
	virtual ~IPokemon() = default;

	/**
	 * Get the species information about the Pokémon in question
	 * @return The species data
	 */
	virtual const FSpeciesData &GetSpecies() const = 0;

	/**
	 * Get the stat information for this Pokémon
	 * @return A reference to the owned stat block
	 */
	virtual const IStatBlock &GetStatBlock() const = 0;
	
};
