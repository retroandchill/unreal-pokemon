// Created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Species/SpeciesData.h"

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
	
};
