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
	
};
