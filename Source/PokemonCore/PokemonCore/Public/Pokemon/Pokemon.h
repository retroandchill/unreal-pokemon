// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "Breeding/PokemonGender.h"
#include "Meta/GenericStaticRegistry.h"
#include "Meta/GenericStaticSharedRegistry.h"
#include "Meta/MetatypeDeclares.h"
#include "Meta/PokeRegistry.h"
#include "Meta/SharedPokeRegistry.h"
#include "Stats/StatBlock.h"

class UPokemonBuilder;
struct FPokemonDTO;
struct FSpeciesData;

/**
 * Abstract interface for a Pokémon object
 */
class POKEMONCORE_API IPokemon {
	DECLARE_ABSTRACT_METATYPE
	
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
	virtual const FSpeciesData& GetSpecies() const = 0;

	/**
	 * Get the gender of the Pokémon in question
	 * @return The Pokémon's gender
	 */
	virtual EPokemonGender GetGender() const = 0;

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
	 * Returns if the Pokémon is currently fainted
	 * @return Is the Pokémon currently fainted
	 */
	virtual bool IsFainted() const = 0;

	/**
	 * Get the stat information for this Pokémon
	 * @return A reference to the owned stat block
	 */
	virtual const IStatBlock& GetStatBlock() const = 0;

	/**
	 * Convert this class into a builder object
	 * @return The builder class to use
	 */
	virtual UPokemonBuilder *ToBuilder() const = 0;

	/**
	 * Check if two Pokémon are the same
	 * @param Other The other Pokémon
	 * @return Are the two Pokémon the same?
	 */
	virtual bool operator==(const IPokemon& Other) const = 0;
};

/**
 * The static registry for all Pokémon
 */
using FPokemonRegistry = TGenericStaticSharedRegistry<IPokemon, const FPokemonDTO&>;